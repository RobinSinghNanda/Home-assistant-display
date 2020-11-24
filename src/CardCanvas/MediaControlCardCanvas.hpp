#ifndef __MEDIACONTROLCARDCANVAS_H__
#define __MEDIACONTROLCARDCANVAS_H__

#include "CardCanvas.hpp"
#include "Canvas/ImageCanvas.hpp"
#include "Canvas/TextCanvas.hpp"
#include "LovelaceConfig/MediaControlCardConfig.hpp"
#include "RowCanvas/MediaPlayerEntityRowCanvas.hpp"
#include "HomeAssistant/Components/MediaPlayer.hpp"
#include "Canvas/SliderCanvas.hpp"

enum MediaControlEvent {
    MEDIA_PLAY_PAUSE,
    MEDIA_SEEK_CHANGED,
    MEDIA_VOLUME_CHANGED,
    MEDIA_VOLUME_MUTE_CHANGED,
    MEDIA_NEXT_TRACK,
    MEDIA_PREV_TRACK
};

class MediaControlCardCanvas : public CardCanvas {
    public:
        MediaControlCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, MediaControlCardConfig * cardConfig, LcdTheme * theme);
        ~MediaControlCardCanvas();
        virtual void update();
    protected:
        DefaultRowConfig * rowConfig;
        MediaPlayerEntityRowCanvas * entityCanvas;
        MediaPlayer * entity;
        MediaControlCardConfig * cardConfig;
        uint16_t buttonColor = TFT_BLACK;
        SliderCanvas * seekSliderCanvas;
        SliderCanvas * volumeSliderCanvas;
        TextCanvas * mediaTitleCanvas;
        ImageCanvas * prevButtonCanvas;
        ImageCanvas * playPauseButtonCanvas;
        ImageCanvas * nextButtonCanvas;
        ImageCanvas * muteButtonCanvas;
        ImageCanvas * repeatButtonCanvas;
        ImageCanvas * shuffleButtonCanvas;
        ImageCanvas * volumeIncButtonCanvas;
        ImageCanvas * volumeDecButtonCanvas;
        void setLayout();
        void setMute(bool mute);
        void setPlaying(bool playing);
};


#endif // __MEDIACONTROLCARDCANVAS_H__