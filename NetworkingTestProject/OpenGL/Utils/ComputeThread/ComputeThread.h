#pragma once
#include <string>
#include <thread>
#include <memory>
#include <iostream>
#include "./../Camera/Camera.h"
#include "./../TerrainBuilder/PerlinTessellation/PerlinTessellationManager.h"
#include <mutex>
#include <queue>
enum LODTYPE {
	HIGHRES,
	LOWRES
};

struct queueProps {
	LODTYPE type;
	int minRange;
	int maxRange;
};

class ComputeThread {
public:
	ComputeThread(std::string Id);
	~ComputeThread();
	void start(GLFWwindow* window);
	void stop();
	void mainComputeLoop();
	void assignSharedPerlinPointer(std::shared_ptr<PerlinTessellationManager> sharedInstance);
	void updateData(CameraInfo info);
	std::mutex& getDataMutex();
	PerlinRegenOutputs getUpdatedData();
	void requestData(LODTYPE type, int min, int max);
	bool isDataReady();
private:
	std::mutex dataMutex;
	PerlinRegenOutputs m_outputs;
	CameraInfo m_ThreadCameraInfo;
	std::string m_ThreadId;
	bool isLoopRunning,m_isDataReady;
	std::unique_ptr<std::thread> threadPtr;
	std::shared_ptr<PerlinTessellationManager> m_sharedPerlinInstance;
	GLFWwindow* m_windowInstance;
	PerlinRegenOutputs m_outGoingBuffer;

	std::queue<queueProps> m_regenRequest;
};