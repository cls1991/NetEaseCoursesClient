
/*
 * 视频信息实体类
 */

using namespace std;

class VideoItem {
public:
    VideoItem(int index, char *vname, char *vurl);
    ~VideoItem();
    char *getVideoName();
    char *getVideoUrl();

private:
    int index; //视频ID
    char *vname; //视频名
    char *vurl; //视频链接
};
