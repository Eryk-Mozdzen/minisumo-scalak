class Point {

    constructor(x,y) {
        this.x = x
        this.y = y
    }

    static distance(p1,p2) {
        return Math.sqrt(Math.pow(p2.x-p1.x,2)+Math.pow(p2.y-p1.y,2))
    }

    static getMouse() {
        let mouseX = event.clientX
        let mouseY = canvas.height - event.clientY
        return new Point(mouseX,mouseY)
    }

    print() {
        console.log("X: "+this.x+" Y: "+this.y)
    }

    show() {
        ctx.beginPath()
        ctx.arc(this.x, this.y, 2, 0, 2*Math.PI, 0)
        ctx.fill()
    }

    rotate(center,angle) {
        //angle *=(Math.PI/180)
        let newX = (this.x-center.x)*Math.cos(angle) - (this.y-center.y)*Math.sin(angle) + center.x
        let newY = (this.x-center.x)*Math.sin(angle) + (this.y-center.y)*Math.cos(angle) + center.y
        this.x = newX
        this.y = newY
    }

    static copy(point) {
        let newX = JSON.parse(JSON.stringify(point.x))
        let newY = JSON.parse(JSON.stringify(point.y))
        return new Point(newX,newY) 
    }

    static angle(p1,p2) {
        let y = p2.y - p1.y
        let x = p2.x - p1.x
        let angle = Math.atan2(y,x)
        if(angle<0) angle = 2*Math.PI-Math.atan2(-y,x)
        return angle
    }

}