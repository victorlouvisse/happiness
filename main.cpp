#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <wchar.h>
#include <sstream>
#include <utility>

#include "trie.h"

using namespace std;

Trie TRIE;

vector<string> explode(string const & s, char delim)
{
  vector<string> result;
  istringstream iss(s);

  for (string token; getline(iss, token, delim); )
  {
    result.push_back(move(token));
  }

  return result;
}

void loadDictionary()
{
  ifstream dictionary( "/home/victor/workspace/Happiness/data/dataset_happiness_avg.csv" );
  if( !dictionary )
  {
    cout << "Erro ao abrir o arquivo de dataset" << endl;
    return;
  }

    //Popula a trie com as palavras do dicionário
  bool dontStopUntilYouGetEnough = true;
  while( dontStopUntilYouGetEnough )
  {
    string str;
    if( !( dictionary >> str ) )
    {
      break;
    }

    string::size_type n = str.find( "," );

    if( n != string::npos )
    {
      string word = str.substr( 0, n );
      double avgVal = stod( str.substr( n+1 ) );
      TRIE.insert( word, avgVal );
    }
  }
}

/* Função necessária para pegar o tamanho das strings que
 * tem caracteres utf8 */
int strlenUtf8( string * str )
{
 const char * s = str->c_str();
 int i = 0;
 int j = 0;
 while ( s[i] )
 {
     /* Filtra caracteres do utf8 e conta somente os que
      * não são */
   if ( ( s[i] & 0xc0 ) != 0x80 )
   {
     j++;
   }

   i++;
 }
 return j;
}

void removeAccents( string * input )
{
  int inputUtf8Size = strlenUtf8( input );

    /* Pela diferença de tamanho sabemos se o input possui caraceteres utf8 */
  bool hasUtf8 = inputUtf8Size != static_cast<int>( input->size() );

  if( hasUtf8 )
  {
    wchar_t accents[] = L"àâêôûãõáéíóúçüÀÂÊÔÛÃÕÁÉÍÓÚÇÜ";
    wchar_t noAccents[] = L"aaeouaoaeioucuAAEOUAOAEIOUCU";
    wchar_t tempWideStr[inputUtf8Size];

        /* Converte o input que é um multi byte string em um wide char string */
    mbstowcs( tempWideStr, input->c_str(), inputUtf8Size );

    for( int i=0; i < inputUtf8Size; i++ )
    {
      for( unsigned j=0; j < wcslen( accents ); j++)
      {
        if( tempWideStr[i] == accents[j] )
        {
          tempWideStr[i] = noAccents[j];
        }
      }
    }

        /* Precisamos reconverter para multi byte string */
    char converted[inputUtf8Size];
    wcstombs( converted, tempWideStr, sizeof( converted ) );

    *input = converted;
  }
}

void toLower( string * input )
{
  transform( input->begin(), input->end(), input->begin(), ::tolower );
}

void removeOthersChars( string * input )
{
  char others[] = "\'\"!@#$%&*()-+={}[]:;?/\\|,.~^`<>";
  int othersSize = 32;
  for( int i=0; i < othersSize; i++)
  {
    if( input->front() == others[i] )
    {
      input->erase( 0, 1 );
    }
    else if( input->back() == others[i] )
    {
      input->erase( input->length()-1, 1 );
    }
  }
}

void processInput( string * input )
{
  /* No dicionário as palavras não estão acentuadas. Por isso, temos que
   * remover os acentos caso seja necessário. */
  removeAccents( input );

  /* Trata as entradas para tirar os caracteres diversos e pontuação */
  removeOthersChars( input );

  /* No dicionário todas as palavras estão em letra minuscula, por isso
   * temos que converter a entrada. Utilizamos algorithm::transform */
  toLower( input );
}

void doTheSearch( string filename )
{
  ifstream file( filename );
  if( !file )
  {
    cout << "Erro ao abrir o arquivo de texto fonte " << filename << endl;
    return;
  }

    //Variáveis para cálculo da média
  int wordsCount = 0;
  double wordsAvgSum = 0;

    //Lê as linhas do texto fonte
  vector<string> words;
  bool dontStopUntilYouGetEnough = true;
  while( dontStopUntilYouGetEnough )
  {
    string str;
    if( !( file >> str ) )
    {
      break;
    }

    words = explode( str, ' ' );

    for( int i=0; i < words.size(); ++i )
    {
      string word = words[i];
      processInput( &word );
      double avgVal = -1;
      if( TRIE.search( word, &avgVal ) )
      {
        wordsAvgSum += avgVal;
        wordsCount++;
      }
      else
      {
        cout << "Ih, não encontrei a palavra " << word << " :(" << endl;
      }
    }
  }

  cout << "Total de palavras: " << wordsCount << " Soma de felicidade: " << wordsAvgSum << endl;

  cout << "Média de felicidade: " << static_cast<double>( wordsAvgSum ) / static_cast<double>( wordsCount ) << endl;
}

int main( int argc, char *argv[] )
{
  if( argc == 1 )
  {
    cout << "Modo de uso:\n./Trie <Um arquivo de texto fonte>" << endl;
    return 1;
  }

    /* Necessário para conversão dos caracteres utf8 */
  setlocale( LC_ALL, "pt_BR.UTF-8" );

  string filename = argv[1];

  loadDictionary();
  doTheSearch( filename );
}