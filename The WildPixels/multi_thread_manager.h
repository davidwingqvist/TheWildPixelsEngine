#pragma once
#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <unordered_map>
#include <queue>

// Scans computer and returns available cores for threads to run on.
const int availableCores = std::thread::hardware_concurrency() - 1;

constexpr unsigned int thread_error = -1;
constexpr unsigned int thread_free = 0;
constexpr unsigned int thread_working = 3;
constexpr unsigned int thread_running = 1;
constexpr unsigned int thread_done = 2;

/*
	Specifies how threads should be used when called upon.
	*POOL means the system will start up a pool with specified amount from parameter intake.
	*INSTANCE will create threads and join them when they are done. This takes more computer processing and preparations with functions used.
	*DEFAULT is used as a placeholder and should be replaced with other options!!
*/
enum class ThreadType
{
	// Create a thread pool and use a Last In First Out(Vector) job assignment.
	POOL_LIFO,

	// Create a thread pool and use a First In First Out(Queue) job assignment.
	POOL_FIFO,

	// One time run each thread and join them when they are done.
	INSTANCE,

	// Will not start any type of threading, only used as a placeholder for Init()
	DEFAULT,

	NROF
};

class MultiThreader
{
private:

	// Normal threads
	std::vector<std::thread> threads;
	int activeThreads;
	static MultiThreader* instance;

	// Pooled Threads
	std::thread* pooledThreads;

	// Jobs to do.
	std::vector<std::function<void()>> jobs;
	std::queue<std::function<void()>> jobs_queue;

	// Running statuses of each thread.
	unsigned int* statuses;

	// Counter for amount of pooled threads.
	unsigned int pooled;

	bool isActive;

	ThreadType type;
	
	/*
		Internal storage for multithreader. unsigned int used for mapping which thread data came from.
	*/
	std::unordered_map<unsigned int, std::string>  texture_data;

	/*
	Start a pool of threads. To assign jobs to these threads please use StartPooled() function. Be sure to not assign more threads than
	available cores. If thats the case the pool will start with highest available "amount" of threads.
	*/
	static void Pool(unsigned int amount);
public:

	/*
		If multithreading is used please remember to put Update() into the "Per Frame" section of the game code.
		To start a thread simply use the Start() function and input a function that returns void and have one parameter intake which is the index.
		On game closage the Multithreader will attempt to join all threads still in progress.
	*/
	MultiThreader();
	~MultiThreader();

	static void Init(unsigned int amount = 1, ThreadType type = ThreadType::DEFAULT);
	static void Destroy();

	/*
		This will initialize a thread with function. An integer is needed as parameter to track index number.
		If the function is successful then it will return the index of the chosen thread. If it is unsuccessful it will return -1.
	*/
	static int Start(void(*function)(int));

	// Update() will join any joinable threads and put a block onto the main thread.
	static void Update();

	// Will return true if any thread is still in the active field, else if no thread is currently running will return false.
	static const bool HasActiveThread();

	// Returns the status of indexed thread.
	static const int GetStatus(int index);

	/*
		Set the status of the indexed thread. Available statuses are:
		thread_error = -1
		thread_free = 0
		thread_running = 1
		thread_done = 2
		You should put thread_running at the start of the thread and thread_done at the absolute bottom.
	*/
	static void SetStatus(unsigned int status, int index);

	// Check for any available jobs inside the system.
	static std::function<void()> CheckJob();

	// Remove the most recent job.
	static void PopJob();

	// Insert a new job into the system.
	static void InsertJob(std::function<void()> job);

	// Used for Threads to check if multithreader is currently active. If not all pooled threads will shutdown.
	static bool IsActive();

	// Allows storage into an unordered_map with index indicating which thread emplaced it and the texture data placed within the keyed spot.
	static void StoreData(char* data, unsigned int index);

	static std::string GetTextureData(unsigned int index); 

	static const int GetAmountOfJobs();
};

/*
	Create a job for the pooled threads, 
	if threads are not activated then this job will run on main thread.
*/
#define THREAD_JOB(class_name, function_name) (MultiThreader::IsActive()) ? MultiThreader::InsertJob(std::bind(&class_name::function_name, &*this)) : class_name::function_name()

/*
	Create a job only if no other jobs are present in the queue.
	If jobs are present then this will run on main thread.
*/
#define THREAD_PRIO_JOB(class_name, function_name) (MultiThreader::GetAmountOfJobs() > 0) ? class_name::function_name : MultiThreader::InsertJob(std::bind(&class_name::function_name, &*this))

/*
	Create a job for the pooled threads,
	This define is suited for singleton functions inside singletons.
*/
#define THREAD_SINGLETON_JOB(class_name, function_name) MultiThreader::InsertJob(std::bind(&class_name::function_name, &*class_name::instance))