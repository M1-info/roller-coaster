#include "headers/Position.h"
#define PI 3.14159265359

using namespace glm;

Position::Position(){
    this.setWagonPosition(vec3(0.0, 0.0, 0.0));
    this.setWagonSpeed(vec3(0.0, 0.0, 0.0));
    this.setWagonWeight(30);
    this.setFrictionCoefficient(1.2);
    this.setDirVector(vec3(0.0, 0.0, 0.0));
}
Position::Position(vec3 startPosition, vec3 startSpeed, vec3 directionVector){
    //conditions initiales
    this.setWagonPosition(startPosition); //position de départ
    this.setWagonSpeed(startSpeed); //vitesse en pixel/s
    this.setWagonWeight(30); //masse du chariot à 30kg
    this.setFrictionCoefficient(1.2); //environ 1.2 kg/m^3 au niveau de la mer et à une température normale
    this.setDirVector(directionVector); //on définit le vecteur direction à l'initialisation de l'instance
}

Position::~Position(){
    //cout << "Destruction instance position." << endl;
}

Position Position::calcul(int tNow, int tNext){
    //initialisation pour faciliter la lecture des calculs
    vec3 acceleration;
    vec3 finalSpeed;
    vec3 initialSpeed = this.getWagonSpeed();
    vec3 finalPosition;
    vec3 initialPosition = this.getWagonPosition();
    float G = this.g;
    vec3 unitVector = this.m_down;
    vec3 directionVector = this.getDirVector();
    float m = this.getWagonWeight();
    float frictionCoefficient = this.getFritionCoefficient();

    //calculs entre t et t + dt

    /*
    g * unitVector car on sait que la pesanteur est dirigé vers le bas
    x,b : b en indice
    (->) : vecteur
    */

    //a,u = -G(->) . u(->) - (k/m) * v
    acceleration = (G * unitVector) * directionVector - (frictionCoefficient / m) * initialSpeed;
    //v,u (t + dt) = v,u (t) + a,u . dt
    finalSpeed = initialSpeed + acceleration * (tNext - tNow);
    //p,u (t + dt) = p,u (t) + v,u . dt . u
    finalPosition = initialPosition + initialSpeed * (tNext - tNow) * directionVector;

    //renvoie
    Position p_finalPosition;
    p_finalPosition.setWagonAcceleration(acceleration);
    p_finalPosition.setWagonSpeed(finalSpeed);
    p_finalPosition.setWagonPosition(finalPosition);
    return p_finalPosition;

    /*
    remarque : 
    si la direction change on doit redéfinir le vecteur direction à chaque fois à la fin de l'utilisation,
    sinon on ne change pas
    */
}