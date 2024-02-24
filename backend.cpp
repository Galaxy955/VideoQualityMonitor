#include <iostream>
#include <sys/stat.h>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <stdlib.h>
}
#include "backend.h"

using namespace std;

void backend::getFilePath(QString q_file_path) {
    // Convert QString to char[].
    string s_file_path = q_file_path.toStdString();
    const char *file_path = s_file_path.c_str();
    // Debug
    // cout << "Received a signal from qml: " << file_path << endl;

    // Get the duration of the video.
    int sec_duration = -1;
    // Open the file by using ffmpeg.
    AVFormatContext *file = nullptr;
    if(avformat_open_input(&file, file_path, nullptr, nullptr) != 0){
        // Failed to open the file.
        throw "Failed to open the file.";
    }else{
        // cout << "Success" << endl; // Debug.
        // Find the stream information of the video.
        if(avformat_find_stream_info(file, nullptr) < 0){
            // Failed.
            throw "Failed to find the stream information of the video.";
        }else{
            int video_stream_index = -1;
            AVCodecParameters *codec_parameters = nullptr;

            for(int i = 0; i < file->nb_streams; i++){
                if(file->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
                    video_stream_index = i;
                    codec_parameters = file->streams[i]->codecpar;
                    break;
                }
            }

            if(video_stream_index == -1){
                // Failed.
                throw "Failed to find the stream information of the video.";
            }

            // Get the duration of the video.
            int64_t duration = file->streams[video_stream_index]->duration;
            AVRational time_base = file->streams[video_stream_index]->time_base;
            sec_duration = duration * time_base.num / time_base.den;
            if(sec_duration == -1){
                throw "Failed to get the time duration of the video.";
            }
            // cout << "The duration of the file (seconds): " << sec_duration << endl; // Debug.

            // Close the file.
            avformat_close_input(&file);
        }
    }

    // Get the file size.
    struct stat file_information;
    stat(file_path, &file_information);
    size_t file_size = file_information.st_size;
    cout << "The size of the file: " << file_size << endl; // Debug.

    // Return the parameters of the video.
    emit returnFileParams(QString::fromStdString(to_string((float)file_size / 1000000)),
                          QString::fromStdString(to_string(sec_duration)));
}

void backend::compute(QString file_size_q, QString duration_q) {
    // Debug.
    // cout << "[Compute] Received signals from qml." << endl;
    // Convert data.
    string file_size_s = file_size_q.toStdString();
    float file_size = atoi(file_size_s.c_str());
    string duration_s = duration_q.toStdString();
    float duration = atoi(duration_s.c_str());
    // cout << "File Size: " << file_size_s << ", " << "Duration: " << duration_s << endl; // Debug
    // Compute the code rate.
    float code_rate = -1;
    code_rate = file_size * 8 / duration;
    // cout << "The code rate of the video (Mbps): " << code_rate << endl; // Debug.
    if(code_rate == -1){
        throw "Failed to compute the code ratio.";
    }else {
        string result = "";
        if (code_rate >= 3.5) {
            result = "极佳";
        } else if (code_rate >= 2.5 && code_rate < 3.5) {
            result = "优秀";
        } else if (code_rate >= 1.1 && code_rate < 2.5) {
            result = "一般";
        } else {
            result = "较差";
        }

        // Convert std::string to QString.
        QString result_q = QString::fromStdString(result);
        QString code_rate_q = QString::fromStdString("码率: " + to_string(code_rate) + "Mbps");

        // Return the result.
        emit returnResult(result_q, code_rate_q);
    }
}
