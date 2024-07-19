<ASSIGNMENT-WORD> ::= <WORD> '=' <WORD>

<REDIRECTION> ::=  '>' <WORD>
                |  '<' <WORD>
                |  '>>' <WORD>
                |  '<<' <WORD>

<SIMPLE-COMMAND-ELEMENT> ::= <WORD>
                          |  <ASSIGNMENT-WORD>
                          |  <REDIRECTION>

<REDIRECTION-LIST> ::= <REDIRECTION>
                    |  <REDIRECTION-LIST> <REDIRECTION>

<SIMPLE-COMMAND> ::=  <SIMPLE-COMMAND-ELEMENT>
                   |  <SIMPLE-COMMAND> <SIMPLE-COMMAND-ELEMENT>

<COMMAND> ::=  <SIMPLE-COMMAND>
            |  <SHELL-COMMAND>
            |  <SHELL-COMMAND> <REDIRECTION-LIST>

<PIPELINE> ::=
          <PIPELINE> '|' <NEWLINE-LIST> <PIPELINE>
       |  <COMMAND>
