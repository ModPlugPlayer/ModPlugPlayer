/*
MPPLogoGraphicsView class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MPPLOGOGRAPHICSVIEW_HPP
#define MPPLOGOGRAPHICSVIEW_HPP

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>

class MPPLogoGraphicsView : public QGraphicsView
{
		Q_OBJECT
	public:
		explicit MPPLogoGraphicsView(QWidget *parent = 0);
		~MPPLogoGraphicsView();
	signals:

	private slots:
		void slotTimer();  /* slot timer overflow handler there will be repainting the widget
								 * */

	private:
		QGraphicsScene      *scene;
		QGraphicsItemGroup  *group_1;
		QGraphicsItemGroup  *group_2;
		qreal currentRotationAngle = 0;
		qreal currentScaleIndex = 0;
		const qreal pi = std::acos(-1);

		/* Timer for delayed rendering.
		 * The fact is that when you create a window and the widget
		 * needs some time to parent layer turned to take
		 * from him adequate width and height settings
		 * */
		QTimer *timer;
		QPixmap *logoPixmap;
		QGraphicsPixmapItem * logoPixmapGraphicsItem[100][100];

	private:
		void resizeEvent(QResizeEvent *event);
};

#endif // MPPLOGOGRAPHICSVIEW_HPP
