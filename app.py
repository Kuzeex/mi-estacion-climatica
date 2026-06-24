from flask import Flask, request, jsonify, send_file
from datetime import datetime

app = Flask(__name__)

# ── Datos en memoria ──
datos_actuales = {
    "temp":    0.0,
    "presion": 0.0,
    "altitud": 0.0,
    "humedad": 0.0,
    "wifi":    0,
    "count":   0
}

historial = []  # últimas 50 lecturas

# ── Página principal ──
@app.route("/")
def index():
    return send_file("clima_dashboard.html")

# ── Recibe datos del Wemos (POST) ──
@app.route("/datos", methods=["POST"])
def recibir_datos():
    data = request.get_json()

    if not data:
        return jsonify({"error": "Sin datos"}), 400

    temp    = float(data.get("temp",    0))
    presion = float(data.get("presion", 0))
    altitud = float(data.get("altitud", 0))
    wifi    = int(data.get("wifi",      0))
    humedad = float(data.get("humedad",   0))

    datos_actuales["temp"]    = round(temp, 1)
    datos_actuales["presion"] = round(presion, 1)
    datos_actuales["altitud"] = round(altitud, 0)
    datos_actuales["wifi"]    = wifi
    datos_actuales["humedad"] = round(humedad, 1)
    datos_actuales["count"]  += 1

    historial.append({
        "time":    datetime.now().strftime("%H:%M"),
        "temp":    round(temp, 1),
        "presion": round(presion, 1),
        "humedad": round(humedad, 1),
        "ok":      temp < 30
    })
    if len(historial) > 50:
        historial.pop(0)

    print(f"[{datetime.now().strftime('%H:%M:%S')}] "
          f"Temp: {temp}°C  Presión: {presion} hPa  "
          f"Altitud: {altitud} m  WiFi: {wifi} dBm  "
          f"Humedad: {humedad}%  WiFi: {wifi} dBm")

    return jsonify({"ok": True}), 200

# ── API para la página web ──
@app.route("/api/datos")
def api_datos():
    return jsonify(datos_actuales)

@app.route("/api/historial")
def api_historial():
    n = int(request.args.get('n', 10))
    return jsonify(historial[-n:])

# ── Arrancar ──
if __name__ == "__main__":
    print("=" * 50)
    print("  Servidor de clima BMP280 iniciado")
    print("  Dashboard: http://localhost:5000")
    print("  Wemos POST a: http://192.168.1.83:5000/datos")
    print("=" * 50)
    app.run(host="0.0.0.0", port=5000, debug=True)