#ifndef WAVE_H
#define WAVE_H

typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk (16 for PCM).
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;

template<typename T>
void write(std::fstream& stream, const T& t)
{
    stream.write((const char*)&t, sizeof(t));
}

class Wave
{
private:
    wav_hdr header;
    ALCsizei samples;
    fstream stream;
public:
    Wave(const std::string filename) : samples(0)
    {
        stream.open(filename, ios::in | ios::out | ios::trunc | ios::binary);

        header.RIFF[0] = 0x52;
        header.RIFF[1] = 0x49;
        header.RIFF[2] = 0x46;
        header.RIFF[3] = 0x46;

        header.WAVE[0] = 0x57;
        header.WAVE[1] = 0x41;
        header.WAVE[2] = 0x56;
        header.WAVE[3] = 0x45;

        header.fmt[0] = 0x66;
        header.fmt[1] = 0x6D; 
        header.fmt[2] = 0x74;
        header.fmt[3] = 0x20;
        header.Subchunk1Size = 16;

        header.AudioFormat = 1;
        header.NumOfChan = 2;
        header.SamplesPerSec = 44100;
        header.bitsPerSample = 16;

        header.bytesPerSec =
            header.SamplesPerSec *
            header.NumOfChan *
            (header.bitsPerSample / 8);

        header.blockAlign =
            header.NumOfChan *
            (header.bitsPerSample / 8);

        header.Subchunk2ID[0] = 0x64;
        header.Subchunk2ID[1] = 0x61;
        header.Subchunk2ID[2] = 0x74;
        header.Subchunk2ID[3] = 0x61;
    }
public:
    void Write(char data)
    {
        stream.seekp(0, ios::end);
        stream.put(data);
    }
    void Out()
    {
        header.Subchunk2Size = samples * header.NumOfChan * header.bitsPerSample / 8;
        header.ChunkSize = 36 + header.Subchunk2Size;

        stream.seekp(0, ios::beg);
        write(stream, header);

        stream.close();
    }
public:
    void Add(ALint* sample)
    {
        this->samples = (ALsizei)sample;
    }
};

#endif // !WAVE_H