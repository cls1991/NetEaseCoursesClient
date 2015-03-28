#include "videoItem.h"

VideoItem::VideoItem(int index, char *vname, char *vurl) :
    index(index),
    vname(vname),
    vurl(vurl)
{
}

VideoItem::~VideoItem() {
}

char *VideoItem::getVideoName() {
    return this->vname;
}

char *VideoItem::getVideoUrl() {
    return this->vurl;
}
