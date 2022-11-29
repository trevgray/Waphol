#include "ProfilerManager.h"
#include "imgui.h"

//#pragma warning(disable:4996)

ProfilerManager::ProfilerManager() {

}

ProfilerManager::~ProfilerManager() {
	profilerResults.clear();
}

//Profiler& ProfilerManager::AddProfiler(const char* name_) {
//	  return profilers["name_"] = Profiler(name_);
//}

void ProfilerManager::RenderProfiler() {
	ImGui::Begin("Profiler"); // Create a window and append into it.
	for (auto profiler : profilerResults) {
		ImGui::Text("%.3fms  %s", profiler.duration, profiler.name);
	}
	profilerResults.clear();
	ImGui::End();
}
