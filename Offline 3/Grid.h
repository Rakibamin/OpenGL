#ifndef _GRID_H
#define _GRID_H
#endif


void blackTile(float a, float b)
{
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS); {
		glVertex3f(a, b, 0);
		glVertex3f(a, -b, 0);
		glVertex3f(-a, -b, 0);
		glVertex3f(-a, b, 0);
	}glEnd();

}

void whiteTile(float a, float b)
{

	glColor3f(1,1,1);
	glBegin(GL_QUADS); {
		glVertex3f(a, b, 0);
		glVertex3f(a, -b, 0);
		glVertex3f(-a, -b, 0);
		glVertex3f(-a, b, 0);
	}glEnd();

}


void horizontalSeries()
{
    whiteTile(10,10);
    glPushMatrix();{
        glTranslated(20,0,0);
        blackTile(10,10);
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-20,0,0);
        blackTile(10,10);
    }glPopMatrix();
}

void verticalSeries()
{
    whiteTile(10,10);
    glPushMatrix();{
        glTranslated(0,20,0);
        blackTile(10,10);
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(0,-20,0);
        blackTile(10,10);
    }glPopMatrix();
}
void baseGrid()
{
    whiteTile(10,10);
    glPushMatrix();{
        glTranslated(20,0,0);
        blackTile(10,10);
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-20,0,0);
        blackTile(10,10);
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(0,20,0);
            blackTile(10,10);
        glPushMatrix();{
            glTranslated(20,0,0);
            whiteTile(10,10);
        }glPopMatrix();
        glPushMatrix();{
            glTranslated(-20,0,0);
            whiteTile(10,10);
        }glPopMatrix();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(0,-20,0);
        blackTile(10,10);
        glPushMatrix();{
            glTranslated(20,0,0);
            whiteTile(10,10);
        }glPopMatrix();
        glPushMatrix();{
            glTranslated(-20,0,0);
            whiteTile(10,10);
        }glPopMatrix();
    }glPopMatrix();

}


void drawGrid()
{
    glPushMatrix();{
        glTranslated(40,40,0);
        whiteTile(10,10);
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-40,40,0);
        whiteTile(10,10);
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(40,-40,0);
        whiteTile(10,10);
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-40,-40,0);
        whiteTile(10,10);
    }glPopMatrix();


    glPushMatrix();{
        glTranslated(0,40,0);
        horizontalSeries();
        glPushMatrix();{
            glTranslated(80,0,0);
            horizontalSeries();
        }glPopMatrix();
        glPushMatrix();{
            glTranslated(-80,0,0);
            horizontalSeries();
        }glPopMatrix();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(0,-40,0);
        horizontalSeries();
        glPushMatrix();{
            glTranslated(80,0,0);
            horizontalSeries();
        }glPopMatrix();
        glPushMatrix();{
            glTranslated(-80,0,0);
            horizontalSeries();
        }glPopMatrix();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(40,0,0);
        verticalSeries();
        glPushMatrix();{
            glTranslated(0,80,0);
            verticalSeries();
        }glPopMatrix();
        glPushMatrix();{
            glTranslated(0,-80,0);
            verticalSeries();
        }glPopMatrix();
    }glPopMatrix();

    glPushMatrix();{
        glTranslated(-40,0,0);
        verticalSeries();
        glPushMatrix();{
            glTranslated(0,80,0);
            verticalSeries();
        }glPopMatrix();
        glPushMatrix();{
            glTranslated(0,-80,0);
            verticalSeries();
        }glPopMatrix();
    }glPopMatrix();



    baseGrid();
    glPushMatrix();{
        glTranslated(80,0,0);
        baseGrid();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(-80,0,0);
        baseGrid();
    }glPopMatrix();
    glPushMatrix();{
        glTranslated(0,80,0);
        baseGrid();
        glPushMatrix();{
            glTranslated(80,0,0);
            baseGrid();
        }glPopMatrix();
        glPushMatrix();{
            glTranslated(-80,0,0);
            baseGrid();
        }glPopMatrix();

    }glPopMatrix();
    glPushMatrix();{
        glTranslated(0,-80,0);
        baseGrid();
        glPushMatrix();{
            glTranslated(80,0,0);
            baseGrid();
        }glPopMatrix();
        glPushMatrix();{
            glTranslated(-80,0,0);
            baseGrid();
        }glPopMatrix();
    }glPopMatrix();
	}
