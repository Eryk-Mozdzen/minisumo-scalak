class Controller {

    constructor(Kp, Ki, Kd) {
        this.Kp = Kp
        this.Ki = Ki
        this.Kd = Kd
        this.deltaTime = dT

        this.integral = 0
        this.lastError = 0
    }

    calculate(processVarible,setPoint) {
        let error = setPoint - processVarible
        let derivative = (error-this.lastError)/this.deltaTime
        this.integral +=(error*this.deltaTime)

        let controlVariable = this.Kp*error + this.Ki*this.integral + this.Kd*derivative

        this.lastError = error

        return controlVariable
    }

    reset() {
        this.integral = 0
        this.lastError = 0
    }

}