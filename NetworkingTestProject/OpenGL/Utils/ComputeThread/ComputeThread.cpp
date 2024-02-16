#include "ComputeThread.h"



ComputeThread::ComputeThread(std::string Id) :
	m_ThreadId(Id), isLoopRunning(false), m_isDataReady(false)
{

}
ComputeThread::~ComputeThread() {
	stop();
}
void ComputeThread::assignSharedPerlinPointer(std::shared_ptr<PerlinTessellationManager> sharedInstance) {
	m_sharedPerlinInstance = sharedInstance;
}
void ComputeThread::start(GLFWwindow* window) {
	m_windowInstance = window;
	// Make sure the thread is not already running
	if (!threadPtr) {
		// Create a thread and call the mainComputeLoop function
		threadPtr = std::make_unique<std::thread>(&ComputeThread::mainComputeLoop, this);
	}
	isLoopRunning = true;
}

void ComputeThread::stop() {
	isLoopRunning = false;

	// If the thread is running, join it
	if (threadPtr && threadPtr->joinable()) {
		threadPtr->join();
	}
}
std::mutex& ComputeThread::getDataMutex() {
	return dataMutex;
}

PerlinRegenOutputs ComputeThread::getUpdatedData() {
	std::cout << "Returning : Vertex Count: " << m_outputs.Vertices.size() << "Index Count: " << m_outputs.Indices.size() << std::endl;
	m_isDataReady = false;
	return std::move(m_outputs);
}
bool ComputeThread::isDataReady() {
	return m_isDataReady;
}
void ComputeThread::mainComputeLoop() {
	while (isLoopRunning) {
		if (!m_regenRequest.empty()) {
			m_isDataReady = false;
			if (m_regenRequest.front().type == LODTYPE::HIGHRES) {
				m_outputs = m_sharedPerlinInstance->RePopulateBuffers(m_ThreadCameraInfo , "HIGHRES", m_regenRequest.front().minRange, m_regenRequest.front().maxRange);
			}
			else if (m_regenRequest.front().type == LODTYPE::LOWRES) {
				m_outputs = m_sharedPerlinInstance->RePopulateBuffers(m_ThreadCameraInfo, "LOWRES", m_regenRequest.front().minRange, m_regenRequest.front().maxRange);
			}
			else {

			}
			m_regenRequest.pop();
			m_isDataReady = true;
		}
		
	}
}

void ComputeThread::updateData(CameraInfo info) {
	m_ThreadCameraInfo = info;
}

void ComputeThread::requestData(LODTYPE type, int min, int max) {
	queueProps properties;
	properties.type = type;
	properties.minRange = min;
	properties.maxRange = max;
	m_regenRequest.push(properties);
}

