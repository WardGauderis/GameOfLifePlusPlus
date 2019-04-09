install qt5:
ubuntu: sudo apt-get install qtbase5-dev

gebruik:
- setCellColor
- getCellColor

met repaint repaint je het scherm (dus alles update tegelijk)

Qt werkt met na u main de app in een oneindige loop te steken, dus de Play knop vervangt de main functie
De onPlay wordt dus opgeroepen als je op play duwt, behandel deze als je main functie ;)