#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <string>

typedef struct {
	char predecessor;
	std::string successor;
} Rule;

class LSystem {

public:
	LSystem(const std::string& axiom);
	LSystem(const std::string& axiom, Rule* rules, int numRules);
	~LSystem();

	void iterate(int iterations);
	void draw(int iterations);

private:
	std::string currentState;
	Rule* rules;
	int numRules;

};

#endif
