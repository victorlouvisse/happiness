#include "node.h"

Node::Node()
    : m_content( ' ' ),
      m_isEnd( false ),
      m_avgVal( -1 )
{
}

Node::~Node()
{
}

Node* Node::findChild(char letter)
{
    for ( unsigned i = 0; i < m_children.size(); i++ )
    {
        Node* tmp = m_children.at(i);

        if ( tmp->content() == letter )
        {
            return tmp;
        }
    }

    return 0;
}
