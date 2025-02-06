#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include "waveout.h"
#include "window.h"
#include "component.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Enola;

class MainComponent : public Window, public Component//主组件类，可以理解为把主窗口当成一个组件
{
private:

	void CreateWindowCB(GLFWwindow* window) override
	{
		SetRootWindow(window);
		SetBounds({ 0,0,GetWindowWidth(),GetWindowHeight() });
	}
	void ResizeWindowCB(int width, int height) override
	{
		SetBounds({ 0,0,width,height });
		Resize();
	}
public:
	MainComponent()
	{

	}
	~MainComponent()
	{

	}
};

class AudioProcessor
{
private:
	WaveOut wo;
	std::thread audioThread;
#define MaxNumSamplesOfBlock 4096	
	int numSamples = 512;
	float bufl[MaxNumSamplesOfBlock];
	float bufr[MaxNumSamplesOfBlock];

	int isClose = 0;
protected:
	void InitAudioProcessor()//waveout start
	{
		wo.Init();
		wo.Start();
		PrepareToPlay(wo.GetSampleRate(), numSamples);
		audioThread = std::thread([this]() {
			while (!isClose)
			{
				ProcessNextBlock(bufl, bufr, numSamples);
				wo.FillBuffer(bufl, bufr, numSamples);
			}
			});
	}
	void CloseAudioProcessor()//waveout stop
	{
		isClose = 1;
		if (audioThread.joinable())audioThread.join();
		wo.Close();
	}
public:
	AudioProcessor()
	{
		memset(bufl, 0, sizeof(bufl));
		memset(bufr, 0, sizeof(bufr));
	}
	~AudioProcessor()
	{
		CloseAudioProcessor();
	}
	void SetNumSamplesOfBlock(int numSamplesOfBlock)
	{
		if (numSamplesOfBlock <= MaxNumSamplesOfBlock)
		{
			numSamples = numSamplesOfBlock;
		}
	}
	virtual void PrepareToPlay(int sampleRate, int numSamplesOfBlock)
	{
		DBG("sampleRate:%d numSamplesOfBlock:%d\n", sampleRate, numSamplesOfBlock);
	}
	virtual void ProcessNextBlock(float* wavbufl, float* wavbufr, int numSamples)
	{
		DBG("ProcessNextBlock\n");
	}
};

class EnolaAudioProcessor : public AudioProcessor
{
private:
	float t = 0;
	float dt = 440.0 / 48000;
public:
	void Init()
	{
		InitAudioProcessor();
	}
	void Close()
	{
		CloseAudioProcessor();
	}
	void PrepareToPlay(int sampleRate, int numSamplesOfBlock) override
	{
		DBG("EnolaAudioProcessor sampleRate:%d numSamplesOfBlock:%d\n", sampleRate, numSamplesOfBlock);
	}
	void ProcessNextBlock(float* wavbufl, float* wavbufr, int numSamples) override
	{
		for (int i = 0; i < numSamples; i++)
		{
			wavbufl[i] = sinf(t * 2.0 * M_PI) * 0.01;
			wavbufr[i] = sinf(t * 2.0 * M_PI) * 0.01;
			t += dt;
			t -= (int)t;
		}
	}
};

class EnolaAudioProcessorEditor : public MainComponent
{
private:
	std::thread RepaintThread;
	int isClose = 1;
public:
	EnolaAudioProcessorEditor()
	{
	}
	void StartAutoReflash()
	{
		RepaintThread = std::thread([this]() {
			while (getWindow())
			{
				Repaint();
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
			});
	}
	void Resize() override
	{
		DBG("Bounds:%d %d %d %d\n", GetBounds().x, GetBounds().y, GetBounds().width, GetBounds().height);
	}
	void Paint(Graphics& g) override
	{
		Enola::Rectangle rect = GetBounds();
		DBG("Rect:%d %d %d %d\n", rect.x, rect.y, rect.width, rect.height);
		for (int i = 0; i < 100; ++i)
		{
			g.SetColor(0xff00ff00);
			g.DrawLine(rand() % 640, rand() % 480, rand() % 640, rand() % 480);
		}
	}
	void Close() override
	{
		isClose = 1;
	}
};

namespace test
{
	class Component
	{
	private:
		std::vector<std::unique_ptr<Component>> childComponents;
	protected:
		void AddAndMakeVisable(Component& p)
		{
			childComponents.emplace_back(&p);
			//todo
		}
	public:
		virtual void Resize(int w, int h)
		{
		}
		virtual void Paint(GLFWwindow* pwh)
		{
		}
	};

	std::mutex WndProcMtx;
	class Window :public Component
	{
	private:
		std::thread wndThread;
		std::queue<std::function<void()>> tasks;
		std::string name;
		int width, height;
		GLFWwindow* pWndHandle = NULL;

		static void ResizeCallback(GLFWwindow* pWnd, int w, int h)
		{
			Window* my = (Window*)glfwGetWindowUserPointer(pWnd);
			my->width = w;
			my->height = h;
			my->Resize(w, h);
		}
		void WndProc()
		{
			WndProcMtx.lock();
			pWndHandle = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
			if (!pWndHandle)
			{
				WndProcMtx.unlock();
				return;
			}
			glfwSetWindowUserPointer(pWndHandle, this);
			glfwSetWindowSizeCallback(pWndHandle, ResizeCallback);
			WndProcMtx.unlock();

			while (!glfwWindowShouldClose(pWndHandle))
			{
				WndProcMtx.lock();
				glfwMakeContextCurrent(pWndHandle);
				while (tasks.size())//外部传入线程内的任务
				{
					auto task = tasks.front();
					tasks.pop();
					task();
				}
				Paint(pWndHandle);
				glfwSwapBuffers(pWndHandle);
				glfwPollEvents();
				WndProcMtx.unlock();
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
		}
	public:
		void Create(std::string& name, int width, int height)
		{
			glfwInit();
			this->name = name;
			this->width = width;
			this->height = height;
			wndThread = std::thread(&Window::WndProc, this);
			Resize(width, height);
		}
		void TransTaskToThread(std::function<void()> func)
		{
			tasks.push(func);
		}
	};
	class AppTest :public Window
	{
	private:
	public:
	};
}

// 立方体顶点数据（边长2，中心在原点）
GLfloat vertices[][3] = {
	{-1.0f, -1.0f, 1.0f},  // 前左下
	{1.0f, -1.0f, 1.0f},   // 前右下
	{1.0f, 1.0f, 1.0f},    // 前右上
	{-1.0f, 1.0f, 1.0f},   // 前左上
	{-1.0f, -1.0f, -1.0f}, // 后左下
	{1.0f, -1.0f, -1.0f},  // 后右下
	{1.0f, 1.0f, -1.0f},   // 后右上
	{-1.0f, 1.0f, -1.0f}   // 后左上
};

// 立方体面定义（每个面4个顶点索引）
GLint faces[][4] = {
	{0, 1, 2, 3}, // 前面
	{1, 5, 6, 2}, // 右面
	{5, 4, 7, 6}, // 后面
	{4, 0, 3, 7}, // 左面
	{3, 2, 6, 7}, // 顶面
	{4, 5, 1, 0}  // 底面
};

// 每个面的颜色（RGBA）
GLfloat colors[][4] = {
	{1.0f, 0.0f, 0.0f, 1.0f}, // 红
	{0.0f, 1.0f, 0.0f, 1.0f}, // 绿
	{0.0f, 0.0f, 1.0f, 1.0f}, // 蓝
	{1.0f, 1.0f, 0.0f, 1.0f}, // 黄
	{0.0f, 1.0f, 1.0f, 1.0f}, // 青
	{1.0f, 0.0f, 1.0f, 1.0f}  // 紫
};
class app_cube :public test::Window
{
private:
	void drawCube() {
		for (int i = 0; i < 6; i++) {
			glBegin(GL_QUADS);
			glColor4fv(colors[i]);
			for (int j = 0; j < 4; j++) {
				glVertex3fv(vertices[faces[i][j]]);
			}
			glEnd();
		}
	}
public:
	void Resize(int w, int h) override
	{
		TransTaskToThread([w, h]() {
			glEnable(GL_DEPTH_TEST);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glViewport(0, 0, w, h);
			float aspect = (float)w / h;
			float fov = 45.0f;
			float nearv = 0.1f;
			float farv = 100.0f;
			float top = nearv * tanf(fov * 3.14159265f / 360.0f);
			float right = top * aspect;
			glFrustum(-right, right, -top, top, nearv, farv);
			});
	}
	void Paint(GLFWwindow* pwh) override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 设置视图矩阵
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -5.0f);  // 相机位置

		// 应用旋转
		float time = glfwGetTime();
		glRotatef(time * 50, 0.5f, 1.0f, 0.0f);

		drawCube();
	}
};
// 甜甜圈参数化生成函数
class Torus {
private:
	const float R = 1.0f;  // 主半径
	const float r = 0.3f;  // 管半径
	const int N = 50;      // 主圆分段数
	const int n = 30;      // 管圆分段数

public:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> texCoords;  // 新增纹理坐标
	std::vector<GLuint> indices;

	Torus() {
		generateVertices();
		generateIndices();
	}

private:
	void generateVertices() {
		for (int i = 0; i <= N; i++) {
			float theta = 2 * M_PI * i / N;
			for (int j = 0; j <= n; j++) {
				float phi = 2 * M_PI * j / n;

				// 顶点坐标
				float x = (R + r * cos(phi)) * cos(theta);
				float y = (R + r * cos(phi)) * sin(theta);
				float z = r * sin(phi);
				vertices.insert(vertices.end(), { x, y, z });

				// 纹理坐标（沿主圆方向）
				texCoords.push_back((float)i / N);  // U坐标从0到1环绕
			}
		}
	}

	void generateIndices() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < n; j++) {
				int first = i * (n + 1) + j;
				int second = first + n + 1;

				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(second + 1);

				indices.push_back(first);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}
	}
};

class app_torus : public test::Window {
private:
	Torus torus;
	float rotationAngle = 0.0f;
	GLuint textureID;  // 纹理ID

	// 生成彩虹纹理
	void createRainbowTexture() {
		const int TEX_SIZE = 256;
		GLubyte texData[TEX_SIZE][3];

		// 生成彩虹色数据
		for (int i = 0; i < TEX_SIZE; ++i) {
			float t = (float)i / TEX_SIZE;
			texData[i][0] = 255 * (0.5f * sin(2 * M_PI * t) + 0.5f);  // R
			texData[i][1] = 255 * (0.5f * sin(2 * M_PI * t + 2 * M_PI / 3) + 0.5f);  // G
			texData[i][2] = 255 * (0.5f * sin(2 * M_PI * t + 4 * M_PI / 3) + 0.5f);  // B
		}

		// 创建纹理
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_1D, textureID);

		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, TEX_SIZE, 0,
			GL_RGB, GL_UNSIGNED_BYTE, texData);
	}

public:
	void Resize(int w, int h) override {
		TransTaskToThread([w, h, this]() {
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_1D);

			// 初始化纹理（仅一次）
			static bool texInitialized = false;
			if (!texInitialized) {
				createRainbowTexture();
				texInitialized = true;
			}

			// 设置投影
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glViewport(0, 0, w, h);
			float aspect = (float)w / h;
			float fov = 45.0f;
			float nearv = 0.1f;
			float farv = 100.0f;
			float top = nearv * tanf(fov * 3.14159265f / 360.0f);
			float right = top * aspect;
			glFrustum(-right, right, -top, top, nearv, farv);
			});
	}

	void Paint(GLFWwindow* pwh) override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 设置视图矩阵
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -5.0f);  // 相机位置

		// 更新旋转
		rotationAngle += 1.0f;
		if (rotationAngle >= 360) rotationAngle -= 360;

		// 启用纹理
		glEnable(GL_TEXTURE_1D);
		glBindTexture(GL_TEXTURE_1D, textureID);

		// 设置材质属性
		glColor3f(1, 1, 1);  // 使用纯白颜色与纹理混合
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// 应用变换
		glPushMatrix();
		glRotatef(rotationAngle, 0.5f, 1.0f, 0.0f);
		drawTexturedTorus();
		glPopMatrix();

		glDisable(GL_TEXTURE_1D);
	}

private:
	void drawTexturedTorus() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// 设置顶点和纹理坐标
		glVertexPointer(3, GL_FLOAT, 0, torus.vertices.data());
		glTexCoordPointer(1, GL_FLOAT, 0, torus.texCoords.data());

		// 绘制
		glDrawElements(GL_TRIANGLES, torus.indices.size(),
			GL_UNSIGNED_INT, torus.indices.data());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
};

app_cube app1;
app_torus app2;
int main()
{
	app1.Create((std::string)"cube", 640, 480);
	app2.Create((std::string)"torus", 640, 480);
	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}