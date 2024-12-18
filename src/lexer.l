%{
#include "ast_nodes/ast.hpp"
#include "parser.hpp"  // Include the header generated by Bison
#include <iostream>
#include <string>

extern YYSTYPE yylval;

int current_line_indent = 0;
int indent_level = 0;

int open_count = 0;
%}

%x leading_tab
%x comment
%s normal

%%

<comment>"\n" { current_line_indent = 0; BEGIN(leading_tab); return NEW_LINE; }
<comment>. {/* ignore comments */}

<leading_tab>\n { current_line_indent = 0; }
<leading_tab>" "{4} { current_line_indent++; }
<leading_tab><<EOF>> {
                        if (indent_level > 0) {
                            indent_level--;
                            return OUTDENT;
                        }
                        yyterminate();
                    }
<leading_tab>.   { 
                    unput(*yytext);
                    if (current_line_indent > indent_level) {
                        indent_level++;
                        return INDENT;
                    } else if (current_line_indent < indent_level) {
                        indent_level--;
                        return DEDENT;
                    } 

                    BEGIN(normal);  
                }


<normal>"\n" { current_line_indent = 0; BEGIN(leading_tab); 
                return NEW_LINE; }

"#" { BEGIN(comment); }

[0-9]+      { yylval.intval = atoi(yytext); 
                return INT_LITERAL; }
[0-9]+\.[0-9]+ { yylval.floatval = atof(yytext); return FLOAT_LITERAL; }

"\"".*"\"" { 
    int len = strlen(yytext);
    memcpy(yytext, yytext + 1, len - 1);
    yylval.strval = strndup(yytext, len - 2); 
    strcpy(yytext,"");
    return STRING_LITERAL; 
    }

true      { yylval.boolval = !strcmp(yytext, "true"); return BOOL_LITERAL; }
false      { yylval.boolval = !strcmp(yytext, "true"); return BOOL_LITERAL; }

AND     { return AND; }
OR      { return OR; }
THEN    { return THEN; }

int      { return INT; }
float    { return FLOAT; }
string  { return STRING; }
bool    { return BOOL; }
void    { return VOID; }
none    { return NONE; }

input     { return INPUT; }
let       { return LET; }
fun       { return FUN; }
return    { return RETURN; }
if        { return IF; }
else      { return ELSE; }
for       { return FOR; }
in        { return IN; }
while     { return WHILE; }
break     { return BREAK; }
continue  { return CONTINUE; }
@if       { return AT_IF; }
@else     { return AT_ELSE; }
@for      { return AT_FOR; }
@load     { return AT_LOAD; }

not        { return NOT; }  

[a-zA-Z_][a-zA-Z0-9_]* { yylval.id = strdup(yytext); return IDENTIFIER; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return STAR; }
"/"         { return SLASH; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
":"         { return COLON; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"="         { return EQUAL; }
","         { return COMMA; }
">"         { return GREATER; }
"<"         { return LESS; }
"?"         { return QUESTION_MARK; }
"%"         { return MOD; }

"**"        { return STAR_STAR; }
"->"        { return TYPE_ARROW; }
"=="        { return EQUAL_EQUAL; }
"!="        { return BANG_EQUAL; }
">="        { return GREATER_EQUAL; }
"<="        { return LESS_EQUAL; }


" "         {/* ignore internal whitespace */}
"\n"        { BEGIN(leading_tab); return NEW_LINE;}

.           { return yytext[0]; }

%%

int yywrap() {
    return 1;
}

void scanner_destroy() {
    yylex_destroy();
}

void scanner_init(const char* code) {
    yy_scan_string(code);
}