import requests
import bs4
import subprocess
from datetime import datetime

def main():
    prediction = get_prediction()
    print(prediction)
    say_prediction(prediction)

def get_prediction():
    res = requests.get('https://tenki.jp/heatshock/5/25/5040/22130/')
    res.raise_for_status()
    soup = bs4.BeautifulSoup(res.text, "html.parser")
    elem = soup.select('.today-weather .heatshock-telop-2')
    return elem[0].getText()
    

def jtalk(t):
    open_jtalk=['open_jtalk']
    mech=['-x','/var/lib/mecab/dic/open-jtalk/naist-jdic']
    htsvoice=['-m','/usr/share/hts-voice/mei/mei_normal.htsvoice']
    speed=['-r','1.0']
    outwav=['-ow','open_jtalk.wav']
    cmd=open_jtalk+mech+htsvoice+speed+outwav
    c = subprocess.Popen(cmd,stdin=subprocess.PIPE)
    c.stdin.write(t.encode())
    c.stdin.close()
    c.wait()
    aplay = ['aplay','-q','open_jtalk.wav']
    wr = subprocess.Popen(aplay)

def say_prediction(prediction):
    d = datetime.now()
    text = '%s月%s日、本日のヒートショック予報をします。本日は、%s、です。' % (d.month, d.day, prediction)
    jtalk(text)

if __name__ == '__main__':
	main()
