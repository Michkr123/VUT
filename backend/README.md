# ITU backend

Jak zprovoznit:
1. Nainstalujte si python 3 (nejspíše už máte)
2. Ve složce backend spusťte příkaz: "python -m venv venv" (vytvoří nové virtuální prostředí ať zbytečně neinstalujete knihovny do systémové verze pythonu)
3. "source venv/Scripts/activate" (aktivuje nové virtuální prostředí v stávajícím příkazovém řádku) (tento příkaz platí pro linux a git bash, pokud používáte windows powershell bude vás zajímat soubor activate.ps1, nechte si cestu doplnit mačkáním tabu)
4. "pip install -r requirements.txt" (nainstaluje požadované knihovny do nově aktivovaného virtuálního prostředí)
5. "python backend.py" (pustí backend)

---

Akce se ukládají do events.json,
recenze do reviews.json,
obrázky jsou ukládány jako base64 string uvnitř events.json,

```
GET /events

Vrátí list akcí v tomto formátu:
{
    "address": "Brno vystaviste",
    "available_tickets": 500,
    "average_rating": null,
    "date_of_creation": "2024-11-03T00:33:14.667210",
    "date_of_event": "2025-06-10 13:00",
    "id": 1,
    "image": true,
    "name": "Letni hudebni festival",
    "review_count": 0
},
```

```
POST /events

Vytvoří novou akci, tělo požadavku vypadá takto:
{
  "name": "string",
  "available_tickets": "number",
  "date_of_event": "string",
  "event_category": "string",
  "address": "string",
  "image": "base64 string (volitelné)"
}
```

> - GET /events/<event_id>
> - vrátí info o události + data obrázku + recenze

> - GET /events/<event_id>/reviews
> - vrátí recenze pro specifikovanou událost

> - GET /events/<event_id>/image
> - vrátí obrázek pro specifikovanou událost

> - PUT /events/<event_id>/image
> - aktualizuje obrázek pro specifikovanou událost


```
POST /events/<event_id>/reviews

Přidá novou recenzi v tomo formátu:

{
  "username": "string",
  "comment": "string",
  "rating": "number (1-5)"
}
```

- Jednotlivé endpointy si můžete ozkoušet pomocí příkazu curl:
- curl http://localhost:5000/events
- curl http://localhost:5000/events/1
- curl http://localhost:5000/events/1/reviews
- curl http://localhost:5000/events/1/image

```
Vytvoření nové akce:

 curl -X POST http://localhost:5000/events \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Letní Festival",
    "available_tickets": 100,
    "date_of_event": "2024-07-15T18:00:00",
    "event_category": "festival",
    "address": "Ulice 123 test",
    "image": "data:image/jpeg;base64,/9j/4AAQSkZJRg..." (toto můžete smazat když zkoušíte endpointy v příkazové řádce, na konverzi obrázků do base64 uděláme funkci ve frontendu)
  }'
```
