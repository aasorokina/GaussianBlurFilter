#ifndef SOURCEIMGWIDGET_H
#define SOURCEIMGWIDGET_H

#include <QtWidgets>

class SourceImgWidget : public QWidget {
  Q_OBJECT

 public:
  explicit SourceImgWidget(QWidget *parent = nullptr);
  ~SourceImgWidget() = default;

  void set_image(const QString &filename);

 private:
  QLabel *img_lbl_;
  QLineEdit *file_name_;

 signals:
};

#endif  // SOURCEIMGWIDGET_H
