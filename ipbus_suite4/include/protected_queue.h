/* this is -*- C++ -*- */
#ifndef QUEUES_PROTECTEDQUEUE_H_
#define QUEUES_PROTECTEDQUEUE_H_


/*
 * $Id: ProtectedQueue.h 102598 2011-09-06 13:50:42Z rhauser $
 */

#include <queue>
//#include <mutex>
//#include <condition_variable>
#include "pthread.h"

/** 
 * A finite-sized queue protected against multiple access from
 * different threads.
 */

template<class T>
class ProtectedQueue {
	public:				// creation/destruction
		/// Create queue with size
		explicit ProtectedQueue(unsigned int size = 0);
		virtual ~ProtectedQueue();
	public:				// access
		/// Insert value into queue. Block if queue is full.
		bool put(const T& value);

		/// Retrieve next value from queue. Block if queue is empty.
		bool get(T& value);

		/** Retrieve next value from queue. If queue is empty
		 * block until at least 'num_ready' entries are available.
		 *
		 * This is a way to avoid wakeups if the cost of a context
		 * switch is too high for a single value to be read from the
		 * queue. Either this method or the previous one should be 
		 * use exclusively if you want to understand your program
		 * behaviour. 
		 */
	bool get(T& value, unsigned int num_ready);

	/// Try to retrieve value from queue. Return true if successfull, false otherwise.
	bool try_get(T& value);

	/// Clear all remaining queue entries
	void clear();

	/// Clear all remaining queue entries, calling
	/// func(x) on each x in the queue.
	template<class FUNC>
		void clear(FUNC func);

	/** Helper function for clear for the common case where T
	 *  is a pointer to some object that should be deleted:
	 *     queue.clear(ProtectedQueue<T>::deleter())
	 */
	struct deleter {
		/// delete the passed pointer
		void operator()(T p) { delete p; }
	};

	/// Wakeup any threads blocked on a get()
	void wakeup();

	/// Met maximum size of queue. Returns 0 if unbounded queue.
	unsigned int max_size() const; 

	/** Get current size of queue.
	 * This value may be wrong by the time you look at
	 * it. It should only be used as a hint and not as
	 * a reliable indication of what's currently in the queue.
	 */
	unsigned int size() const;

	private:
	pthread_mutex_t               m_mutex;
	pthread_cond_t   m_not_full;
	pthread_cond_t   m_not_empty;
	unsigned int        m_max_size;
	std::queue<T,std::deque<T> > m_queue;
	bool                m_wakeup;
	unsigned int        m_wait_for;
};

#include "protected_queue.cxx"
#endif // QUEUES_PROTECTEDQUEUE_H_



