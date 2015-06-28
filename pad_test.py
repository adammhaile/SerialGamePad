import time
from bibliopixel.animation import *
from bibliopixel import *
from SerialGamePad import SerialGamePad

class d(dict):
    def __init__(self, *a, **k):
        super(d, self).__init__(*a, **k)
        self.__dict__ = self
        for k in self.__dict__:
        	if isinstance(self.__dict__[k], dict):
        		self.__dict__[k] = d(self.__dict__[k])
        	elif isinstance(self.__dict__[k], list):
        		for i in range(len(self.__dict__[k])):
        			if isinstance(self.__dict__[k][i], dict):
        				self.__dict__[k][i] = d(self.__dict__[k][i])
    def upgrade(self, a):
        for k,v in a.items():
            if not k in self:
                self[k] = v

#animation class
class GamePadTest(BaseMatrixAnim):
    def __init__(self, led):
        super(GamePadTest, self).__init__(led)
        self._pad = SerialGamePad()

    def step(self, amt=1):
        b = d(self._pad.readState())
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
driver = DriverVisualizer(num=0, width=64, height=8, pixelSize=10, port=1618, stayTop=True)
led = LEDMatrix(driver, threadedUpdate=True)
anim = GamePadTest(led)

try:
    anim.run(fps=10)
except KeyboardInterrupt:
    led.all_off()
    led.update()
    time.sleep(1)
