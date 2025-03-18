#include "../include/gaussianfilter.h"

GaussianFilter::GaussianFilter() : result_image_(QPixmap()) {
  source_image_ = std::make_unique<QImage>();
  blur_radius_ = 0;
}

bool GaussianFilter::set_source_image(const QString &filepath) {
  bool error = false;
  source_image_.reset();
  source_image_ = std::make_unique<QImage>(filepath);

  if (source_image_->isNull()) {
    error = true;
  }

  return error;
}

void GaussianFilter::set_blur(int r) {
  result_image_ = QPixmap();
  is_cancelled_ = false;

  blur_radius_ = r;
  int kernel_size = ceil(2 * blur_radius_) + 1;

  if (source_image_->isNull() || r <= 0) {
    emit blur_completed();
    return;
  }

  emit progress_updated(0);

  std::unique_ptr<double[]> kernel = std::make_unique<double[]>(kernel_size);
  generate_kernel(kernel.get(), blur_radius_);

  if (is_cancelled_) {
    emit_completion_signals();
    return;
  }

  QImage first = apply_blur_one_dimension(kernel.get(), *source_image_, true);

  if (is_cancelled_) {
    emit_completion_signals();
    return;
  }

  QImage second = apply_blur_one_dimension(kernel.get(), first, false);

  result_image_ = QPixmap::fromImage(second);

  emit_completion_signals();
}

void GaussianFilter::cancel_blur() { is_cancelled_ = true; }

int GaussianFilter::get_blur_radius() { return blur_radius_; }

void GaussianFilter::generate_kernel(double kernel[], int sigma) {
  int size = ceil(2 * sigma) + 1;
  int center = size / 2;
  double sum = 0.0;

  for (int i = 0; i < size; i++) {
    int x = i - center;

    kernel[i] = (1.0 / (sqrt(2.0 * M_PI * sigma * sigma))) *
                exp(-(x * x) / (2.0 * sigma * sigma));
    sum += kernel[i];
  }

  for (int i = 0; i < size; i++) {
    kernel[i] /= sum;
  }
}

QImage GaussianFilter::apply_blur_one_dimension(double kernel[], QImage &source,
                                                bool horisontal) {
  QImage result(source);
  int height = source.height();
  int width = source.width();
  int kernel_size = ceil(2 * blur_radius_) + 1;
  int radius = kernel_size / 2;

  for (int i = 0; i < height && !is_cancelled_; i++) {
    for (int j = 0; j < width && !is_cancelled_; j++) {
      double sum_red = 0.0, sum_green = 0.0, sum_blue = 0.0, sum_alpha = 0.0;
      double kernel_sum = 0.0;
      int curr_pixel;

      for (int k = -radius; k <= radius; k++) {
        curr_pixel = horisontal ? qBound(0, j + k, width - 1)
                                : qBound(0, i + k, height - 1);

        double weight = kernel[k + radius];
        kernel_sum += weight;

        QRgb *source_line = reinterpret_cast<QRgb *>(
            source.scanLine(horisontal ? i : curr_pixel));

        int index = horisontal ? curr_pixel : j;

        sum_red += qRed(source_line[index]) * weight;
        sum_green += qGreen(source_line[index]) * weight;
        sum_blue += qBlue(source_line[index]) * weight;
        sum_alpha += qAlpha(source_line[index]) * weight;
      }
      QRgb *result_line = reinterpret_cast<QRgb *>(result.scanLine(i));

      result_line[j] =
          qRgba(qBound(0, static_cast<int>(sum_red / kernel_sum), 255),
                qBound(0, static_cast<int>(sum_green / kernel_sum), 255),
                qBound(0, static_cast<int>(sum_blue / kernel_sum), 255),
                qBound(0, static_cast<int>(sum_alpha / kernel_sum), 255));
    }
    int progress =
        static_cast<int>(((i + 1) * 50.0 / height) + (horisontal ? 0 : 50));
    emit progress_updated(progress);
  }
  return result;
}

QPixmap GaussianFilter::get_blurred_image() { return result_image_; }

void GaussianFilter::emit_completion_signals() {
  emit progress_updated(100);
  emit blur_completed();
}
