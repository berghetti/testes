def interval_confiance( data ):
    Z = 1.96 # nivel de confiança 95%
    avg = sum(data) / len(data)

    sum_ = 0
    for i in range(len(data)):
        sum_ += math.pow( data[i] - avg, 2 )

    desvio = math.sqrt( sum_ / len(data) )
    margin_error = Z * ( desvio / math.sqrt(len(data) ) ) # intervalo de confiança

    avg = round(avg, 4)
    margin_error = round(margin_error, 4)
    return avg, margin_error

