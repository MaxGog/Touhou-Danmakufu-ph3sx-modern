#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>

namespace gstd {
	//****************************************************************************
	//Thread
	//****************************************************************************
	class Thread {
	public:
		enum Status {
			RUN,
			STOP,
			REQUEST_STOP,
		};
	private:
		void _StaticRun();
	protected:
		std::thread thread_;
		std::atomic<Status> status_;
		std::mutex completionMutex_;
		std::condition_variable completionCondition_;
		bool completed_;

		virtual void _Run() = 0;
	public:
		Thread();
		virtual ~Thread();

		virtual void Start();
		virtual void Stop();
		bool IsStop();
		uint32_t Join(int mills = -1);

		Status GetStatus() { return status_.load(); }
	};

	//****************************************************************************
	//CriticalSection
	//****************************************************************************
	class CriticalSection {
		std::recursive_mutex mutex_;
	public:
		CriticalSection();
		~CriticalSection();

		void Enter();
		void Leave();
	};

	//****************************************************************************
	//Lock
	//	Mutex locking based on a critical section object
	//****************************************************************************
	class Lock {
	protected:
		CriticalSection* cs_;
	public:
		Lock(CriticalSection& cs) { cs_ = &cs; cs_->Enter(); }
		Lock(CriticalSection* cs) { cs_ = cs; cs_->Enter(); }
		virtual ~Lock() { cs_->Leave(); }
	};

	//****************************************************************************
	//StaticLock
	//	Basic mutex locking
	//****************************************************************************
	class StaticLock {
	protected:
		static std::recursive_mutex* mtx_;
	public:
		StaticLock();
		~StaticLock();
	};

	//****************************************************************************
	//ThreadSignal
	//	Wrapper for thread event signaling
	//****************************************************************************
	class ThreadSignal {
		std::mutex mutex_;
		std::condition_variable condition_;
		bool manualReset_;
		bool signaled_;
	public:
		ThreadSignal(bool bManualReset = false);
		virtual ~ThreadSignal();

		uint32_t Wait(int mills = -1);
		void SetSignal(bool bOn = true);
	};
}