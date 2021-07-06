let dT = 1

let target = []
let position = []
let time = 0

let pid = new Controller(0.1,0,1)
let robot = new Robot(10,environment(0),0,0)

for(let t=0; t<canvas.width/dT; t++) {
    let correct = environment(time)
    robot.action(pid.calculate(robot.position,correct))
    robot.move()

    target.push(correct)
    position.push(robot.position)

    time +=dT
}

time = 0
for(let t=1; t<canvas.width/dT; t++) {
    line(new Point(time-dT,target[t-1]),new Point(time,target[t]),3,new Color(0,0,255))
    line(new Point(time-dT,position[t-1]),new Point(time,position[t]),3,new Color(0,255,0))
    time +=dT
}

function environment(input) {
    if(0<=input && input<50) return 0
    //if(50<=input && input<400) return 500
    //if(400<=input && input<800) return 300
    //return 400
    //return (Math.sin(input/50)*50)+250

    return 1/(Math.abs(Math.sin(0.03*input))+0.01)+500
}