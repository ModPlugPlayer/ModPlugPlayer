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
