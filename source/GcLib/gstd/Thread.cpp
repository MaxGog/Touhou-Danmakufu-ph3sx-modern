#include "Thread.hpp"

using namespace gstd;

namespace {
	constexpr uint32_t threadWaitTimeout = 258;
	constexpr uint32_t threadWaitFailed = UINT32_MAX;
}

//*******************************************************************
//Thread
//*******************************************************************
Thread::Thread() {
	status_ = STOP;
	completed_ = true;
}
Thread::~Thread() {
	this->Stop();
	this->Join();
}
void Thread::_StaticRun() {
	try {
		_Run();
	}
	catch (...) {
		//Errors unhandled
	}
	status_ = STOP;
	{
		std::lock_guard<std::mutex> lock(completionMutex_);
		completed_ = true;
	}
	completionCondition_.notify_all();
}
void Thread::Start() {
	if (thread_.joinable()) {
		this->Stop();
		this->Join();
	}
	{
		std::lock_guard<std::mutex> lock(completionMutex_);
		completed_ = false;
	}
	status_ = RUN;
	try {
		thread_ = std::thread(&Thread::_StaticRun, this);
	}
	catch (...) {
		status_ = STOP;
		{
			std::lock_guard<std::mutex> lock(completionMutex_);
			completed_ = true;
		}
		throw;
	}
}
void Thread::Stop() {
	Status expected = RUN;
	status_.compare_exchange_strong(expected, REQUEST_STOP);
}
bool Thread::IsStop() {
	return status_.load() == STOP;
}
uint32_t Thread::Join(int mills) {
	if (!thread_.joinable())
		return 0;
	if (thread_.get_id() == std::this_thread::get_id())
		return threadWaitFailed;

	{
		std::unique_lock<std::mutex> lock(completionMutex_);
		if (mills < 0) {
			completionCondition_.wait(lock, [this] { return completed_; });
		}
		else if (!completionCondition_.wait_for(lock, std::chrono::milliseconds(mills),
			[this] { return completed_; })) {
			return threadWaitTimeout;
		}
	}

	thread_.join();
	return 0;
}

//*******************************************************************
//CriticalSection
//*******************************************************************
CriticalSection::CriticalSection() {
}
CriticalSection::~CriticalSection() {
}
void CriticalSection::Enter() {
	mutex_.lock();
}
void CriticalSection::Leave() {
	mutex_.unlock();
}

//*******************************************************************
//StaticLock
//*******************************************************************
std::recursive_mutex* StaticLock::mtx_ = nullptr;
StaticLock::StaticLock() {
	if (mtx_ == nullptr) {
		mtx_ = new std::recursive_mutex();
	}
	mtx_->lock();
}
StaticLock::~StaticLock() {
	mtx_->unlock();
}

//*******************************************************************
//ThreadSignal
//*******************************************************************
ThreadSignal::ThreadSignal(bool bManualReset) {
	manualReset_ = bManualReset;
	signaled_ = false;
}
ThreadSignal::~ThreadSignal() {
}
uint32_t ThreadSignal::Wait(int mills) {
	std::unique_lock<std::mutex> lock(mutex_);
	bool signaled;
	if (mills < 0) {
		condition_.wait(lock, [this] { return signaled_; });
		signaled = true;
	}
	else {
		signaled = condition_.wait_for(lock, std::chrono::milliseconds(mills),
			[this] { return signaled_; });
	}
	if (signaled && !manualReset_)
		signaled_ = false;
	return signaled ? 0 : threadWaitTimeout;
}
void ThreadSignal::SetSignal(bool bOn) {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		signaled_ = bOn;
	}
	if (bOn) {
		if (manualReset_)
			condition_.notify_all();
		else
			condition_.notify_one();
	}
}
