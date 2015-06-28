import time
from bibliopixel.animation import *
from bibliopixel import *
from bibliopixel.util import d
from SerialGamePad import SerialGamePad

#animation class
class GamePadTest(BaseMatrixAnim):
    def __init__(self, led):
        super(GamePadTest, self).__init__(led)
        self._pad = SerialGamePad()

    def step(self, amt=1):
        b = d(self._pad.getKeys())
        self._led.drawText("U" if b.UP else " ", x=0)
        self._led.drawText("D" if b.DOWN else " ", x=6)
        self._led.drawText("L" if b.LEFT else " ", x=12)
        self._led.drawText("R" if b.RIGHT else " ", x=18)

        self._led.drawText(chr(253) if b.RED else " ", x=24, color = colors.Red)

        self._led.drawText("A" if b.A else " ", x=30, color = colors.Green)
        self._led.drawText("B" if b.B else " ", x=36, color = colors.Red)
        self._led.drawText("X" if b.X else " ", x=42, color = colors.Blue)
        self._led.drawText("Y" if b.Y else " ", x=48, color = colors.Yellow)

import bibliopixel.log as log
log.setLogLevel(log.logging.DEBUG)

from bibliopixel.drivers.visualizer import DriverVisualizer
driver = DriverVisualizer(num=0, width=64, height=8, pixelSize=20, port=1618, stayTop=True)
led = LEDMatrix(driver, threadedUpdate=False)
anim = GamePadTest(led)

try:
    anim.run()#fps=60)
except KeyboardInterrupt:
    led.all_off()
    led.update()
    time.sleep(1)
