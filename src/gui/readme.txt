install qt5:
ubuntu: sudo apt-get install qtbase5-dev
arch: nothing

gebruik:
- setCellColor(int x, int y, ui::Color)
- getCellColor(int x, int y)
- execute(int ticks)

met repaint() repaint je het scherm (dus alles update tegelijk)
gebruik ui::Grid::simulation als main
