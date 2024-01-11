#pragma once
#include <iostream>
#include<queue>
#include<thread>
#include<condition_variable>
#include<vector>
#include<functional>
#include<future>
using namespace std;

typedef function<void()>task_type;
typedef void(*FuncType)(int, int);
typedef future<void> res_futur_type;

template<class T>
class BlockedQueue
{
public:
	void push(T& item)
	{
		lock_guard<mutex>l(m_locker);
		m_task_queue.push(move(item));
		m_notifier.notify_one();
	}
	void pop(T& item)
	{
		unique_lock<mutex>l(m_locker);
		if (m_task_queue.empty())
		{
			m_notifier.wait(l, [this], { return !m_task_queue.empty(); });
			item =move( m_task_queue.front());
			m_task_queue.pop();
		}
	}
	bool fast_pop(T& item)//неблокирующий метод получения элемента из очереди, возвращает ложь, если очередь пуста
	{
		lock_guard<mutex>l(m_locker);
		if (m_task_queue.empty())return false;
		item =move(m_task_queue.front());
		m_task_queue.pop();
		return true;
	}
private:
	mutex m_locker;
	queue<T>m_task_queue;
	condition_variable m_notifier;
};

class OPTIMYSEDThreadPool
{
public:
	OPTIMYSEDThreadPool();
	void  start();
	res_futur_type  stop();
	res_futur_type push_task(FuncType f, int id, int arg);
	void threadFunc(int qindex);
private:
	struct TaskWithPromise
			{
				task_type task;
				promise<void> prom;
			} ;
	int m_thread_count;//количество потоков в запущенном состоянии
	vector<thread>m_threads;
	vector<BlockedQueue<TaskWithPromise>>m_thread_queues;//очередь задач для потоков
	int m_qindex;//  переменная для равномерного распределения задач     
																														//volatile bool m_work;
	condition_variable m_event_holder;
	mutex m_locker;

};

class RequestHandler
{
public:
	RequestHandler();
	~RequestHandler();

	res_futur_type pushRequest(FuncType f, int id, int arg);
private:
	OPTIMYSEDThreadPool m_tpool;

};
