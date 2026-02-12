from flask import Flask, request, jsonify
import os
import threading
import json

app = Flask(__name__)

# Habilitar CORS para permitir peticiones desde la red/ESP o desde páginas web
try:
    from flask_cors import CORS
    CORS(app)
except Exception:
    print("flask_cors no disponible: instala 'flask-cors' si necesitas CORS en el navegador")

# WebSocket servidor (para ESP32 NetworkManager en puerto 5050)
ws_clients = []
WS_AVAILABLE = False

try:
    from websocket_server import WebsocketServer
    WS_AVAILABLE = True
except ImportError:
    print("⚠ websocket-server no disponible. ESP32 no podrá conectar vía WebSocket.")
    print("  Instala con: pip install websocket-server")


def new_client_handler(client, server):
    """Se ejecuta cuando se conecta un cliente WebSocket"""
    print(f"[WS] Cliente conectado: {client['address']}")
    ws_clients.append(client)
    # Enviar confirmación al cliente (ESP32)
    server.send_message(client, json.dumps({"type": "connected", "message": "Conectado al servidor Xiaozhi"}))


def message_handler(client, server, message):
    """Procesa mensajes del WebSocket"""
    print(f"[WS] Mensaje recibido: {message}")
    try:
        data = json.loads(message)
        comando = data.get('type', data.get('comando'))
        
        if comando == 'hablar':
            texto = data.get('texto', '')
            os.system(f'say "{texto}"')
            print(f"🔊 Xiaozhi dice: {texto}")
            response = json.dumps({"type": "status", "result": "hablando"})
            server.send_message(client, response)
        
        elif comando == 'abrir_ae':
            os.system('open -a "Adobe After Effects 2024"')
            print("🎬 Abriendo After Effects")
            response = json.dumps({"type": "status", "result": "abriendo AE"})
            server.send_message(client, response)
        
        else:
            response = json.dumps({"type": "error", "message": "Comando no reconocido"})
            server.send_message(client, response)
    except json.JSONDecodeError:
        print("[WS] Error: JSON inválido")
        server.send_message(client, json.dumps({"type": "error", "message": "JSON inválido"}))


def client_left_handler(client, server):
    """Se ejecuta cuando se desconecta un cliente"""
    print(f"[WS] Cliente desconectado: {client['address']}")
    if client in ws_clients:
        ws_clients.remove(client)


def run_websocket_server():
    """Corre el servidor WebSocket en un thread aparte"""
    if not WS_AVAILABLE:
        print("[WS] WebSocket deshabilitado")
        return
    
    server = WebsocketServer(host='0.0.0.0', port=5050, loglevel='WARNING')
    server.set_fn_new_client(new_client_handler)
    server.set_fn_message_received(message_handler)
    server.set_fn_client_left(client_left_handler)
    
    print("[WS] Servidor WebSocket iniciado en ws://0.0.0.0:5050")
    server.serve_forever()


@app.route('/', methods=['GET'])
def index():
    html = """
    <!doctype html>
    <html lang="es">
      <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width,initial-scale=1">
        <title>Xiaozhi Brain</title>
        <style>body{font-family:sans-serif;margin:20px}</style>
      </head>
      <body>
        <h1>🧠 Xiaozhi Brain</h1>
        <p><strong>Servidores:</strong></p>
        <ul>
          <li>🌐 HTTP en puerto 5000 (Flask)</li>
          <li>⚡ WebSocket en puerto 5050 (ESP32)</li>
        </ul>
        
        <h2>REST API</h2>
        <form id="frm">
          <input id="texto" placeholder="Texto a decir" value="Hola desde navegador">
          <button type="button" onclick="enviarREST()">Enviar 'hablar' (REST)</button>
        </form>
        <pre id="resp"></pre>
        
        <script>
        async function enviarREST(){
          const txt=document.getElementById('texto').value;
          const r=await fetch('/xiaozhi',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({comando:'hablar',texto:txt})});
          const j=await r.text();
          document.getElementById('resp').textContent = r.status + '\\n' + j;
        }
        </script>
      </body>
    </html>
    """
    return html, 200, {'Content-Type': 'text/html; charset=utf-8'}


@app.route('/xiaozhi', methods=['POST', 'GET'])
def xiaozhi_command():
    if request.method == 'GET':
        return jsonify(status="ready", methods=["POST"], example={"comando":"hablar","texto":"Hola"}), 200

    data = request.get_json(silent=True)
    if not data:
        print("Request body no contiene JSON válido")
        return jsonify(status="error", message="JSON inválido o vacío"), 400

    comando = data.get('comando')
    texto = data.get('texto', '')

    print(f"Recibido comando REST: {comando} texto: {texto}")

    if comando == 'hablar':
        os.system(f'say "{texto}"')
        print(f"🔊 Xiaozhi dice: {texto}")
        return jsonify(status="hablando"), 200

    if comando == 'abrir_ae':
        os.system('open -a "Adobe After Effects 2024"')
        return jsonify(status="abriendo AE"), 200

    return jsonify(status="comando no reconocido"), 400


if __name__ == '__main__':
    # Iniciar WebSocket en thread separado
    if WS_AVAILABLE:
        ws_thread = threading.Thread(target=run_websocket_server, daemon=True)
        ws_thread.start()
    
    # Flask en el thread principal
    print("🚀 Iniciando Xiaozhi Brain...")
    app.run(host='0.0.0.0', port=5000, debug=False)