#include "protected_queue.h"
#include "Event.h"
	template<class T>
ProtectedQueue<T>::ProtectedQueue(unsigned int size):m_max_size(size),m_wakeup(false),m_wait_for(0)
{
	pthread_mutex_init(&m_mutex,NULL);
	pthread_cond_init(&m_not_full,NULL);
	pthread_cond_init(&m_not_empty,NULL);
}

	template<class T>
ProtectedQueue<T>::~ProtectedQueue()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_not_full);
	pthread_cond_destroy(&m_not_empty);
}

	template<class T>
bool ProtectedQueue<T>::put(const T& value)
{
	//std::unique_lock<std::mutex> lock(m_mutex);
	pthread_mutex_lock(&m_mutex);
	while(!m_wakeup && (m_max_size != 0) && (m_queue.size() == m_max_size)) {
		//m_not_full.wait(lock);
		pthread_cond_wait(&m_not_full,&m_mutex);
	}

	if(m_wakeup) {
		pthread_mutex_unlock(&m_mutex);
		return false;
	}

	m_queue.push(value);

	if(m_queue.size() >= m_wait_for) {
		pthread_cond_signal(&m_not_empty); 
		//m_not_empty.notify_one();
	}

	pthread_mutex_unlock(&m_mutex);
	return true;
}

	template<class T>
bool ProtectedQueue<T>::get(T& value)
{
	{
		//std::unique_lock<std::mutex> lock(m_mutex);
		pthread_mutex_lock(&m_mutex);
		while(!m_wakeup && m_queue.size() == 0) {
			pthread_cond_wait(&m_not_empty,&m_mutex);
			//m_not_empty.wait(lock);
		}

		if(m_wakeup) {
			pthread_mutex_unlock(&m_mutex);
			return false;
		}

		value = m_queue.front();
		m_queue.pop();

	}

	//m_not_full.notify_one();
	pthread_cond_signal(&m_not_full);
	pthread_mutex_unlock(&m_mutex);

	return true;
}


	template<class T>
bool ProtectedQueue<T>::get(T& value, unsigned int num_ready)
{
	{
		//std::unique_lock<std::mutex> lock(m_mutex);
		pthread_mutex_lock(&m_mutex);
		while(!m_wakeup && !m_queue.size()) {
			m_wait_for = num_ready;
			pthread_cond_wait(&m_not_empty,&m_mutex);
			//m_not_empty.wait(lock);
		}

		if(m_wakeup) {
			pthread_mutex_unlock(&m_mutex);
			return false;
		}

		value = m_queue.front();
		m_queue.pop();

		m_wait_for = 0;
	}
	pthread_cond_signal(&m_not_full);
	pthread_mutex_unlock(&m_mutex);
	//m_not_full.notify_one();

	return true;
}

	template<class T>
bool ProtectedQueue<T>::try_get(T& value)
{       
	//m_mutex.lock();
	pthread_mutex_lock(&m_mutex);

	if(m_queue.size() > 0) {
		value = m_queue.front();
		m_queue.pop();
		//m_mutex.unlock();
		pthread_mutex_unlock(&m_mutex);
		pthread_cond_signal(&m_not_full);
		//m_not_full.notify_one();
		return true;
	} else {
		pthread_mutex_unlock(&m_mutex);
		//m_mutex.unlock();
		return false;
	}
}

	template<class T>
void ProtectedQueue<T>::clear()
{
	pthread_mutex_lock(&m_mutex);
	// std::unique_lock<std::mutex> lock(m_mutex);
	while(!m_queue.empty()) {
		m_queue.pop();
	}
	pthread_cond_signal(&m_not_full);
	pthread_mutex_unlock(&m_mutex);
	// m_not_full.notify_one();
	m_wakeup = false;
}

template<class T>
	template<class FUNC>
void ProtectedQueue<T>::clear(FUNC func)
{
	//std::unique_lock<std::mutex> lock(m_mutex);
	pthread_mutex_lock(&m_mutex);
	while(!m_queue.empty()) {
		func(m_queue.front());
		m_queue.pop();
	}
	pthread_cond_signal(&m_not_full);
	pthread_mutex_unlock(&m_mutex);
	//m_not_full.notify_one();
	m_wakeup = false;
}



	template<class T>
void ProtectedQueue<T>::wakeup()
{
	//std::unique_lock<std::mutex> lock(m_mutex);
	pthread_mutex_lock(&m_mutex);
	m_wakeup = true;
	pthread_cond_broadcast(&m_not_empty);
	pthread_cond_broadcast(&m_not_full);
	//m_not_empty.notify_all();
	//m_not_full.notify_all();
	pthread_mutex_unlock(&m_mutex);
}

template<class T>
unsigned int ProtectedQueue<T>::max_size() const
{
	return m_max_size;
}

template<class T>
unsigned int ProtectedQueue<T>::size() const
{
	return m_queue.size();
}



























