#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include <vector>

enum cellState{VISITED, UNVISITED, VISITING};

static float Vertices[] = {
	// positions    
	 1.0f,  1.0f, 0.0f,   // top right
	 1.0f,  0.0f, 0.0f,   // bottom right
	 0.0f,  0.0f, 0.0f,   // bottom left
	 0.0f,  1.0f, 0.0f    // top left 
};

//static float Vertices[] = {
//	// positions   
//	 0.5f,  0.5f, 0.0f,   // top right
//	 0.5f, -0.5f, 0.0f,   // bottom right
//	-0.5f, -0.5f, 0.0f,   // bottom left
//	-0.5f,  0.5f, 0.0f   // top left 
//};

struct Cell
{
	int weight;
	cellState m_State;

	Shader* cell_shader;
	Transform trans;

	VertexArray va; // (GenVertexArray)
	VertexBuffer* vb; // (Gen, Bind, Data Implement)
	VertexBufferLayout layout;

	Cell() {
		weight = 0; 
		m_State = UNVISITED;
		// Init Shader
		cell_shader = Shader::FromGLSLTextFiles(
			"assets/shaders/Cell.vert.glsl",
			"assets/shaders/Cell.frag.glsl"
		);
		// INIT VBO and IBO
		vb = new VertexBuffer(Vertices, sizeof(Vertices)); // init vertex buffer (Gen, Bind, Data Implement)
		// Create layout
		layout.Push<float>(3); // 3 floats per vertex for position
		va.AddBuffer((*vb), layout);
	}
};


class Grid
{
public:
	
	Grid() {
		// Default states
	}
	// init Grid - fill grid with defaults with correct size.
	void Init(int row, int col) {
		m_State.grid = std::vector<std::vector<Cell>>(row, std::vector<Cell>(col, Cell()));
	}
	// Draw Grid - using passed in params, render the grid
	void DrawGrid(Renderer &renderer, GLCore::Utils::OrthographicCameraController &m_CameraController) {
		cell.cell_shader->use();
		cell.cell_shader->SetUniformMatrix4fv("viewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
		cell.trans.setPosition({ 0.0,0.0,0.0 });
		renderer.DrawLineLoop(cell.va, *(cell.cell_shader), cell.trans, 4 );
	}

	// Getters //
	std::vector<std::vector<Cell>> getGrid() { return m_State.grid; }


private:
	Cell cell;
	struct gridState {
		std::vector<std::vector<Cell>> grid;

	};
	gridState m_State;

};

