import { Hill } from './hill.js';
import { SheepController } from './sheep-controller.js';

class App {
    constructor() {
        this.canvas = document.createElement('canvas'); //create canvas
        this.ctx = this.canvas.getContext('2d');
        document.body.appendChild(this.canvas);     //append to body

        this.hills = [
            new Hill('#fd6bea', 0.2, 12),
            new Hill('#ff59c2', 0.5, 8),
            new Hill('#ff4674', 1.4, 6)
        ];

        this.SheepController = new SheepController();

        window.addEventListener('resize', this.resize.bind(this), false);   //get screen size
        this.resize();      //then resize according to the screen size

        requestAnimationFrame(this.animate.bind(this));
    }

    resize() {
        this.stageWidth = document.body.clientWidth;
        this.stageHeight = document.body.clientHeight;

        this.canvas.width = this.stageWidth * 2;        //double the dimensions for possible display issues with different devices/screens
        this.canvas.height = this.stageHeight * 2;
        this.ctx.scale(2, 2);

        for (let i = 0; i < this.hills.length; i++) {
            this.hills[i].resize(this.stageWidth, this.stageHeight);
        }

        this.SheepController.resize(this.stageWidth, this.stageHeight);
    }

    animate(t) {
        requestAnimationFrame(this.animate.bind(this));

        this.ctx.clearRect(0, 0, this.stageWidth, this.stageHeight);

        let dots;
        for (let i = 0; i < this.hills.length; i++) {
            dots = this.hills[i].draw(this.ctx);
        }

        this.SheepController.draw(this.ctx, t, dots);
    }
}

window.onload = () => {
    new App();
};