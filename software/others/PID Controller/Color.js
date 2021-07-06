class Color {

    constructor(r,g,b) {
        this.r = r
        this.g = g
        this.b = b
    }

    get() {
        return 'rgb('+this.r+', '+this.g+', '+this.b+')'
    }

}