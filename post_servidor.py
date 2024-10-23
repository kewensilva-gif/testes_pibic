import cv2
import time
import urllib.request
import numpy as np
import requests

# url do esp
url = 'http://192.168.1.14/cam-hi.jpg'
cv2.namedWindow("EspCam live", cv2.WINDOW_AUTOSIZE)

# endpoint da API
server_url = 'http://localhost:8000/api/imagens'

while True:
    try:
        # pega a imagem da stream
        img_resp = urllib.request.urlopen(url)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        im = cv2.imdecode(imgnp, -1)

        # exibe a imagem localmente
        cv2.imshow('EspCam live', im)

        # transforma a imagem em jpg
        _, img_encoded = cv2.imencode('.jpg', im)

        response = requests.post(server_url, files={'image': ('image.jpg', img_encoded.tobytes(), 'image/jpeg')})

        # time.sleep(1)
        if response.status_code == 200:
            print('Imagem enviada com sucesso!')
        else:
            print(f'Falha ao enviar imagem: {response.status_code}')

        # pressione 'q' para sair do loop
        key = cv2.waitKey(5)
        if key == ord('q'):
            break

    except Exception as e:
        print(f'Erro: {e}')
        break

cv2.destroyAllWindows()
