#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
	// Initialize the matrix stack with the identity matrix.
    m_matrices.push_back( glm::mat4( 1.0 ) );
}

void MatrixStack::clear()
{
	// Revert to just containing the identity matrix.
    m_matrices.clear();
    m_matrices.push_back( glm::mat4( 1.0 ) );
}

glm::mat4 MatrixStack::top()
{
	// Return the top of the stack, doesnt remove
    return m_matrices.back();
}

void MatrixStack::push( const glm::mat4& m )
{
	// Push m onto the stack.
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
    
    //Doing multiplication
    glm::mat4 top = m * m_matrices.back();
    m_matrices.push_back(top);
}

void MatrixStack::pop()
{
	// Remove the top element from the stack (doesn't return)
    m_matrices.pop_back();
}
