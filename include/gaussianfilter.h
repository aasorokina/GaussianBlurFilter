#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H

#include <QtConcurrent/QtConcurrent>
#include <QtWidgets>
#include <atomic>
#include <functional>

class GaussianFilter : public QObject {
  Q_OBJECT

public:
  GaussianFilter();
  ~GaussianFilter() = default;
  bool set_source_image(const QString &filepath);
  int get_blur_radius();
  void generate_kernel(double *kernel, int sigma);
  QImage apply_blur_one_dimension(double kernel[], QImage &source,
                                  bool horisontal);
  QPixmap get_blurred_image();

public slots:
  void set_blur(int r);
  void cancel_blur();
  void emit_completion_signals();

signals:
  void blur_completed();
  void progress_updated(int percent);

private:
  std::unique_ptr<QImage> source_image_;
  QPixmap result_image_;
  int blur_radius_;
  std::atomic<bool> is_cancelled_;
};

#endif // GAUSSIANFILTER_H
