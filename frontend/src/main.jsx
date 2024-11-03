import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import './index.css'

import Header from "./components/Header";
import EventsPage from "./EventsPage";
import EventDetails from "./components/EventDetails";
import WorkerPage from "./WorkerPage";
import AdminPage from "./AdminPage";

createRoot(document.getElementById('root')).render(
  <StrictMode>
    {/* Routování mezi jednotlivými časti aplikace */}
    <Router>
      <div className="min-h-screen bg-gray-50">
        <Header />
        <Routes>
          <Route path="/" element={<EventsPage />} />
          <Route path="/event/:id" element={<EventDetails />} />
          <Route path="/worker" element={<WorkerPage />} />
          <Route path="/admin" element={<AdminPage />} />
        </Routes>
      </div>
    </Router>
  </StrictMode>,
)
