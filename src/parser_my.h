#ifndef _PARSER_MY_H_
#define _PARSER_MY_H_

#include <vector.h>

#define WITHIN(V, L, H) ((V) >= (L) && (V) <= (H))
#define NUMERIC(a) WITHIN(a, '0', '9')

struct parserCode
{
  char letter;
  double codenum;
};

class Parser
{
private:
  Vector<parserCode> parserCodeVect;

public:
  Parser()
  {
    parserCode array[] = {0};
    parserCodeVect.Assign(array, 1);
    parserCodeVect.Clear();
  }

  void parse(const char *p)
  {
    parserCodeVect.Clear();

    bool hasMore = true;
    do
    {

      parserCode result;
      result.letter = '?';
      result.codenum = 0;

      if (*p == 0 || *p == ';')
      {
        return;
      }

      // Skip spaces
      while (*p == ' ')
        ++p;

      if (*p == 0 || *p == ';')
      {
        return;
      }

      const char letter = *p >= 'a' && *p <= 'z' ? *p & 0xDF : *p; // to upper case
      p++;

      double codenum = 0;
      double dot = 1;
      double minus = 1;
      if (*p == '-')
      {
        minus = -1;
        p++;
      }

      if (NUMERIC(*p))
      {
        do
        {
          codenum *= 10.0;
          codenum += *p++ - '0';
        } while (NUMERIC(*p));

        if (*p == '.')
        {
          p++;
          if (NUMERIC(*p))
            do
            {
              dot *= 10.0;
              codenum += (*p++ - '0') / dot;
            } while (NUMERIC(*p));
        }
      }

      result.letter = letter;
      result.codenum = codenum * minus;
      parserCodeVect.PushBack(result);

      if (*p == 0 || *p == ';')
      {
        return;
      }

    } while (hasMore);
  }

  bool hasCommand(char c)
  {
    for (int i = 0; i < parserCodeVect.Size(); i++)
      if (parserCodeVect[i].letter == c)
        return true;

    return false;
  }

  double getCommand(char c)
  {
    for (int i = 0; i < parserCodeVect.Size(); i++)
      if (parserCodeVect[i].letter == c)
        return parserCodeVect[i].codenum;

    return 0;
  }
};

void parse_my(const char *p, void (*send)(const char *));

extern Parser parser;

#endif // _PARSER_MY_H_