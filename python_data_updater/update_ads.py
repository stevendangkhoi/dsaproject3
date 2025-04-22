import os
import pandas as pd
import json

file = os.path.dirname(os.path.abspath(__file__))
csv = os.path.join(file, 'youtube.csv')
json_old = os.path.join(file, 'super_bowl_ads.json')
json_updated = os.path.join(file, 'super_bowl_ads_updated.json')

df = pd.read_csv(csv)
with open(json_old, 'r', encoding='utf-8') as f:
    ads = json.load(f)

df['view_count_int'] = df['view_count'].fillna(-1).astype(int)
lookup = {
    (int(r['year']), r['brand'], int(r['view_count_int'])): r
    for _, r in df.iterrows()
}

for entry in ads:
    key = (
        int(entry['Year']),
        entry['Brand'],
        int(entry['Data']['Viewership']['Views'])
    )
    if key in lookup:
        r = lookup[key]
        entry['Data']['Content'] = {
            'Funny': bool(r['funny']),
            'Shows Product': bool(r['show_product_quickly']),
            'Patriotic': bool(r['patriotic']),
            'Celebrity': bool(r['celebrity']),
            'Danger': bool(r['danger']),
            'Animals': bool(r['animals']),
            'Sexual': bool(r['use_sex'])
        }
    else:
        entry['Data']['Content']['_note'] = 'No matching CSV flags found'

with open(json_updated, 'w', encoding='utf-8') as f:
    json.dump(ads, f, indent=2)

print(f"Wrote Updated Super Bowl Ads JSON to {json_updated}")
