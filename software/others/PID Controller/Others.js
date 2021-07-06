let canvas = document.getElementById('canvas')
let ctx = canvas.getContext('2d')

ctx.transform(1, 0, 0, -1, 0, canvas.height)

function rand(min,max) {
    return Math.random()*(max-min) + min;
}

function randInteger(min,max) {
    return Math.floor(Math.random()*(max+1-min) + min);
}

function relu(input) {
    if(input>0) return input;
    else return 0;
}

function softmax(input) {
    return (1/(1+Math.exp(-input)));
}

function getRandomName() {
    let vowels = ['e','u','o','a','i','y'];
    let consonants = ['w','r','t','p','s','d','f','g','h','j','k','l','z','b','n','m','c'];
    let name = "";
    let size = randInteger(2, 5);
    for (let i=0; i<size; ++i) {
        name +=consonants[randInteger(0, consonants.length-1)];
        name +=vowels[randInteger(0, vowels.length-1)];
    }
    return name;
}

function mutateName(name1,name2) {
    let split1 = randInteger(1, name1.length-1);
    let split2 = randInteger(1, name2.length-1);
    let name = "";
    for (let i=0; i<split1; i++) name +=name1[i];
    for (let i=split2; i<name2.length; i++) name +=name2[i];

    return name;
}

function map(x, inputMIN, inputMAX, outputMIN, outputMAX) {
    return (x-inputMIN)*(outputMAX-outputMIN)/(inputMAX-inputMIN)+outputMIN
}

let mouse

document.getElementById("canvas").onmousemove = function() {
    mouse = Point.getMouse()
}

function margeArrays(arr1,arr2) {
    let out = []
    for(let object of arr1) out.push(JSON.parse(JSON.stringify(object)))
    for(let object of arr2) out.push(JSON.parse(JSON.stringify(object)))
    return out
}

function line(begin,end,width,color) {
    ctx.lineWidth = width
    ctx.strokeStyle = color.get()
    ctx.beginPath()
    ctx.moveTo(begin.x,begin.y)
    ctx.lineTo(begin.x,begin.y)
    ctx.lineTo(end.x,end.y)
    ctx.stroke()
}

function circle(position,radius,color) {
    ctx.fillStyle = color.get()
    ctx.beginPath()
    ctx.moveTo(position.x,position.y)
    ctx.arc(position.x,position.y,radius,0,Math.PI*2,true)
    ctx.fill()
}