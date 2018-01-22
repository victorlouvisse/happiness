#include "trie.h"
#include "node.h"

Trie::Trie()
    : m_root( new Node() )
{
}

Trie::~Trie()
{
    //Destrutor
}

void Trie::insert( string str, double avgVal )
{
    Node* current = m_root;

    if ( str.length() == 0 )
    {
        current->setIsEnd();
        return;
    }

    for ( unsigned i = 0; i < str.length(); i++ )
    {
        Node* child = current->findChild( str[i] );

        if ( child != NULL )
        {
            current = child;
        }
        else
        {
            Node* tmp = new Node();
            tmp->setContent( str[i] );
            current->appendChild( tmp );
            current = tmp;
        }

        if ( i == str.length() - 1 )
        {
            current->setIsEnd();
            current->setAvgValue( avgVal );
        }
    }
}

bool Trie::search( string str, double * avgVal )
{
    Node* current = m_root;

    while ( current != NULL )
    {
        for ( unsigned i = 0; i < str.length(); i++ )
        {
            Node* tmp = current->findChild( str[i] );

            if ( tmp == NULL )
            {
                return false;
            }
            current = tmp;
        }

        if ( current->isEnd() )
        {
            *avgVal = current->getAvgValue(); 
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
