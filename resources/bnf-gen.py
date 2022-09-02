import sys
import re

used_rules = set()
defined_rules = set()

def to_rules(lines):
    arr = []
    for line in lines:
        if "::=" in line:
            if len(arr)>0:
                yield arr 
            arr = []

        arr.append(line)

def transform(rule):
    
    rule = list(map((lambda line: 
        line.split()
    ), rule))

    defined_rules.add(rule[0][0][1:-1])

    def norm(token):
        if token == "::=":
            return "<-"
        
        if rule[0][0] != "<inclusive-or-expression>" and token == "|":
            return "/"

        if len(token) < 5:
            return "'" + token + "'"

        # no token with length greater than 5 has a <> pattern naturally
        if not ("<" in token and ">" in token):
            return "'" + token + "'"

        if "{" in token and "}" in token:
            token = token.replace("{", "(")
            token = token.replace("}", ")")

        used_rules.add(list(re.findall(r"<(.*)>", token))[0])

        token = token.replace("<", "")
        token = token.replace(">", "")
        
        return token

    rule = list(map((lambda line: 
        list(map((
            lambda token: norm(token)
        ), line))
    ),rule))

    return rule

# Missing rules
# {'<integer-constant>', '<character-constant>', '<floating-constant>', '<identifier>', '<enumeration-constant>', '<jump-statement>', '<string>'}

if __name__ == "__main__":
    print(r"# https://cs.wmich.edu/~gupta/teaching/cs4850/sumII06/The%20syntax%20of%20C%20in%20Backus-Naur%20form.htm")
    for rule in to_rules(sys.stdin.readlines()):
        rule = transform(rule)
        for line in rule:
            for token in line:
                print(token, end=" ")
            print()

    #print(used_rules - defined_rules)

    # no support for other bases
    print("integer-constant <- < '0' / [1-9][0-9]* >")
    
    # Not a comprehensive floating constant match, but works for now
    print("floating-constant <- < [+-]?([0-9]+([.][0-9]*)?|[.][0-9]+) >")

    # This is really sus char matching
    print("character-constant <- < '(.*)' >")

    # This is really sus string matching
    print('character-constant <- < "(.*)" >')

    

    print("identifier <- < [a-zA-Z_][a-zA-Z0-9_]* >")

    # these last ones not really sure

    print ("enumeration-constant <- identifier")

    print ("jump-statement <- 'goto' identifier")

    print (r"%whitespace <- [ \t\v\n\f\r]*")