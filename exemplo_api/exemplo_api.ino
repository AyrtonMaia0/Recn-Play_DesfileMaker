@app.route('/setamodelo', methods=['GET','POST'])
def setamodelo():
    dados = request.get_json()
    valor = dados["valor"]
    global statusModeloUm
    statusModeloUm = valor
    return {"Stat modelo um": statusModeloUm}

@app.route('/statusmodeloum', methods=['GET', 'POST'])
def testaget():
    global statusModeloUm
    valor = statusModeloUm
    statusModeloUm = "Null"
    return {"Modelo Um": str(valor)}
