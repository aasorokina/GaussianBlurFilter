FROM fffaraz/qt:static
LABEL maintainer="buggkell"
RUN apt-get update && apt-get upgrade -y
WORKDIR /
COPY ./ /home/GaussianBlurFilter
COPY ./resources/fonts/segoeui.ttf /usr/local/Qt-5.15.9/lib/fonts/segoeui.ttf
RUN cd /home/GaussianBlurFilter && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    cmake --build .
ENTRYPOINT ["/home/GaussianBlurFilter/build/gaussian_filter"]