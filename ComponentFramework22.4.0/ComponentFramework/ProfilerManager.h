#pragma once
#include <chrono>
#include <iostream>
#include <vector>
//ref: https://www.youtube.com/watch?v=YbYV8rRo9_A

//#define PROFILE_SCOPE(name) Profiler profiler##__LINE__(name, [&](ProfilerResult profileResult) { profilerResults.push_back(profileResult); })

struct ProfilerResult {
	const char* name;
	float duration;
};

template<typename Fn> 
class Profiler {
public:
	//Profiler() {}
	Profiler(const char* name_, Fn&& func_) : func(func_) {
		name = name_;
		stopped = false;
		startTimePoint = std::chrono::high_resolution_clock::now();
		duration = 0.0f;
	}

	~Profiler() {
		if (!stopped) {
			Stop();
		}
	}

	void Stop() {
		std::chrono::time_point<std::chrono::steady_clock> endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		stopped = true;

		float duration = (end - start) * 0.001f;
		//std::cout << name << " " << duration << "ms" << std::endl;
		func({ name, duration });
	}

	const char* GetName() { return name; }
	float GetDuration() { return duration; }
private:
	const char* name;
	std::chrono::time_point<std::chrono::steady_clock> startTimePoint;
	bool stopped;
	float duration;
	Fn func;
};

class ProfilerManager {
public:
	ProfilerManager();
	~ProfilerManager();

	void AddProfiler(const char* name_) {
		Profiler profiler(name_, [&](ProfilerResult profileResult) { profilerResults.push_back(profileResult); });
	}

	//Profiler& AddProfiler(const char* name_);

	void RenderProfiler();

	//std::vector<ProfilerResult> profilerResults;
private:
	//std::unordered_map<const char*, Profiler> profilers;
	std::vector<ProfilerResult> profilerResults;
};