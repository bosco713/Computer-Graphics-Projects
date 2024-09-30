#include "LSystem.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerglobals.h"
#include <FL/gl.h>

// CLASS LSystem METHODS

LSystem::LSystem(const std::string& axiom) : currentState(axiom), rules(nullptr), numRules(0) { }

LSystem::LSystem(const std::string& axiom, Rule* rules, int numRules) : currentState(axiom) {
    this->numRules = numRules;
    if (numRules > 0) {
        this->rules = new Rule[numRules];
        for (int i = 0; i < numRules; i++)
            this->rules[i] = rules[i];
    }
    else
        this->rules = nullptr;
}

LSystem::~LSystem() {
    delete[] rules;
}

void LSystem::iterate(int iterations) {
    for (int i = 0; i < iterations; i++) {
        std::string nextState;
        for (char current : currentState) {
            bool found = false;
            for (int j = 0; j < numRules; j++)
                if (current == rules[j].predecessor) {
                    nextState += rules[j].successor;
                    found = true;
                    break;
                }
            if (!found)
                nextState += current;
        }
        currentState = nextState;
    }
}

void LSystem::draw(int iterations) {
    double scale = 1.0;
    double pi = 360 * (ModelerApplication::Instance()->GetFrame() / 49.0f) * M_PI / 180;

    iterate(iterations);
    glPushMatrix();
    glTranslated(VAL(XPOS), VAL(YPOS) + 2 * sin(pi), VAL(ZPOS));
    glRotated((double)VAL(LSYSTEM_INITIAL_ANGLE), 0.0, 0.0, 1.0);

    for (char current : currentState) {
        switch (current) {
        case 'F':
            glBegin(GL_LINES);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(0.0, 1.0 * VAL(LSYSTEM_LENGTH_FACTOR) * scale, 0.0);
            glEnd();
            glTranslated(0.0, 1.0 * VAL(LSYSTEM_LENGTH_FACTOR) * scale, 0.0);
            break;
        case '+':
            glRotated((double)VAL(LSYSTEM_INCREMENT_ANGLE) + (double)VAL(LSYSTEM_INCREMENT_ANGLE) * sin(pi / 2) * sin(pi / 2), 0.0, 0.0, 1.0);
            break;
        case '-':
            glRotated(-(double)VAL(LSYSTEM_INCREMENT_ANGLE) - (double)VAL(LSYSTEM_INCREMENT_ANGLE) * sin(pi / 2) * sin(pi / 2), 0.0, 0.0, 1.0);
            break;
        case '[':
            glPushMatrix();
            break;
        case ']':
            glPopMatrix();
            break;
        default:
            break;
        }
    }
    glPopMatrix();
}


