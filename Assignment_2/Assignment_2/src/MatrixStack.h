#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class MatrixStack
{
public:
	MatrixStack();
	void clear();
	glm::mat4 top();
	void push( const glm::mat4& m );
	void pop();

private:

	std::vector< glm::mat4 > m_matrices;

};

#endif // MATRIX_STACK_H
