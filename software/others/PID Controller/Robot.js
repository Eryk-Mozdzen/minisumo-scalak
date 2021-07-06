class Robot {

    constructor(mass,position,velocity,acceleration) {
        this.mass = mass
        this.position = position
        this.velocity = velocity
        this.acceleration = acceleration
    }

    action(force) {
        this.acceleration = (force/this.mass)
    }
    
    move() {
        this.velocity +=this.acceleration*dT
        this.position +=this.velocity*dT
    }

}