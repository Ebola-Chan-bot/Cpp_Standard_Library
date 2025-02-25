#ifdef ARDUINO_ARCH_AVR
#include <unordered_set>
#include <cerrno>
#include <pthread.h>
#include <Arduino.h>
static std::unordered_set<pthread_mutex_t *> Mutexes;
static bool BaselineInterrupt;
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	bool const CurrentInterrupt =
#ifdef ARDUINO_ARCH_AVR
		SREG & (1 << SREG_I)
#endif
#ifdef ARDUINO_ARCH_SAM
				   !__get_PRIMASK()
#endif
		;
	noInterrupts();
	if (Mutexes.empty())
		BaselineInterrupt = CurrentInterrupt;
	else if (Mutexes.contains(mutex))
		return EDEADLK;
	Mutexes.insert(mutex);
	return 0;
}
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	Mutexes.erase(mutex);
	if (Mutexes.empty() && BaselineInterrupt)
		interrupts();
	return 0;
}
int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	return pthread_mutex_lock(mutex);
}
#endif