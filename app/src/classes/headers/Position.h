#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <math.h>
#include <iostream>

/*
    La classe Position permet de calculer la position, la vitesse et l'accélération du chariot en fonction
    des forces appliquées.
    On prend en compte la force de frottement de l'air, la gravité et la force de réaction.

    Utilisation : 
        On déclare une position : Position x;
        On peut le déclarer avec une position initiale, vitesse initiale et un vecteur direction, 
        on peut aussi lui affecter ces paramètres avec les mutateurs 
        - setWagonPosition(vec3)
        - setWagonSpeed(vec3)
        - setDirVector(vec3)
        Ensuite on utilise la méthode calcul entre t et t + dt, par exemple
        entre t(initial) = 0 et t(final) = 10
        Donc l'intervalle [t(initial), t(final)] = 10 - 0 = 10
            x = Position.calcul(t(initial), t(final));
        Puis on lui affecte le vecteur direction
            x.setDirVector(direction);
        On met ça dans une boucle pour calculer sa position à chaque frame
    
    Remarque : 
        On n'utilise pas les variables de forces car leur formule sont déjà incluse et
    aussi certaine force peut-être disparaissent car si on projette les forces sur l'axe du circuit
    on a plus la force de réaction (car il est égal à 0, car le vecteur de réaction est normale à l'axe du circuit)
*/

class Position{
    private:
        glm::vec3 m_dirVector; //vecteur unitaire représentant la direction du circuit au niveau du chariot
        //float m_frictionForce; //force de frottement
        float m_frictionCoefficient; //coefficient de frottement
        //float m_wagonForce; //force du chariot
        //float m_gravityForce; //force de la gravité
        glm::vec3 m_wagonPosition; //position du chariot
        glm::vec3 m_wagonSpeed; //vitesse du chariot
        glm::vec3 m_wagonAcceleration; //acceleration du chariot
        float m_wagonWeight; //masse du chariot
        //float m_reactionForce; //force de la réaction
    
    public:
        //constante de pesanteur
        const float g = 9.81;

        //vecteur
        glm::vec3 m_down(0.0, -1.0, 0.0); //vecteur unitaire dirigée vers le bas

        //constructeurs & destructeur
        Position();
        Position(vec3 startPosition, vec3 startSpeed, vec3 directionVector);
        ~Position();

        //accesseurs
        glm::vec3 getDirVector() const {return m_dirVector;}
        //float getFrictionForce() const {return m_frictionForce;}
        //float getWagonForce() const {return m_wagonForce;}
        //float getGravityForce() const {return m_gravityForce;}
        glm::vec3 getWagonPosition() const {return m_wagonPosition;}
        glm::vec3 getWagonSpeed() const {return m_wagonSpeed;}
        glm::vec3 getWagonAcceleration() const {return m_wagonAcceleration;}
        //glm::vec3 getReactionForce() const {return m_reactionForce;}
        glm::vec3 getWagonWeight() const {return m_wagonWeight;}
        float getFritionCoefficient() const {return m_frictionCoefficient;}

        //mutateurs
        void setDirVector(glm::vec3 dirVector) {m_dirVector = dirVector;}
        //void setFrictionForce(float frictionForce) {m_frictionForce = frictionForce;}
        //void setWagonForce(float wagonForce) {m_wagonForce = wagonForce;}
        //void setGravityForce(float gravityForce) {m_gravityForce = gravityForce;}
        void setWagonPosition(glm::vec3 wagonPosition) {m_wagonPosition = wagonPosition;}
        void setWagonSpeed(glm::vec3 wagonSpeed) {m_wagonSpeed = wagonSpeed;}
        void setWagonAcceleration(glm::vec3 wagonAcceleration) {m_wagonAcceleration = wagonAcceleration;}
        //void setReactionForce(float reactionForce) {m_reactionForce = reactionForce;}
        void setWagonWeight(float wagonWeight) {m_wagonWeight = wagonWeight;}
        void setFrictionCoefficient(float frictionCoefficient) {m_frictionCoefficient = frictionCoefficient;}

        //méthodes
        Position calcul(int tNow, int tNext);
};