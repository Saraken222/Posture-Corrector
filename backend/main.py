from fastapi import FastAPI
from pydantic import BaseModel
from fastapi.middleware.cors import CORSMiddleware


app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)
events= []

class PostureEvent(BaseModel):
    timestamp: str
    pitch: float
    status: str

@app.get("/")
def root():
    return {"message": "Posture API running"}

@app.post("/events")
def update_event(event: PostureEvent):
    events.append(event)
    return {"message": "event added", "total_events": len(events)}

@app.get("/stats")
def get_stats():
    total = len(events)
    if total ==0: return {"message": "no data yet"}

    bad_events = [e for e in events if e.status != "GOOD"]
    bad_total = len(bad_events)
    percent_bad = (bad_total/ total) * 100
    
    current_posture = "Good"
    if events[-1].status != "GOOD": current_posture = "Poor"

    return{
        "total_events": total,
        "bad_events": bad_total,
        "percent_bad": percent_bad,
        "current_posture": current_posture
    }