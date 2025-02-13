/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Marek Konečný (xkonec85)
Description: Komponenta pro formulář pro vytvoření nové akce
*/
import React, { useState } from 'react';
import { Button, Dialog, DialogActions, DialogContent, DialogTitle, TextField, Select, MenuItem, InputLabel, FormControl } from '@mui/material';
import DraggableCategory from './DraggableCategory';
import CategoryDropZone from './CategoryDropZone';
import { EVENT_BUTTONS } from '../EventsPage';

// NewEventForm component
const NewEventForm = ({ open, onClose, onEventCreated }) => {
  const [newEvent, setNewEvent] = useState({ image: '', name: '', event_category: '', organizer: '', region: '', city: '', address: '', date_of_event: '', description: '' }); // State to store new event data
  const [previewUrl, setPreviewUrl] = useState(null); // State to store image preview URL

  // Handle image change
  const handleImageChange = (e) => {
    const file = e.target.files[0];
    if (file) {
      const reader = new FileReader();

      reader.onloadend = () => {
        setNewEvent({ ...newEvent, image: reader.result }); // Set the image data in newEvent state
        setPreviewUrl(reader.result); // Set the preview URL
      };

      reader.readAsDataURL(file); // Read the file as a data URL
    }
  };

  // Handle category drop
  const handleDropCategory = (category) => {
    setNewEvent((prevEvent) => ({ ...prevEvent, event_category: category })); // Set the dropped category in newEvent state
  };

  // Handle form submission
  const handleSubmitEvent = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch(`http://localhost:5000/events`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(newEvent), // Send newEvent data as JSON
      });
      if (response.ok) {
        onEventCreated(); // Call onEventCreated callback
        onClose(); // Close the form
      }
    } catch (error) {
      console.error('Error submitting event:', error);
    }
  };

  return (
    <Dialog open={open} onClose={onClose} maxWidth="md" fullWidth>
      <DialogTitle>Vytvořit novou akci</DialogTitle>
      <DialogContent>
        <form onSubmit={handleSubmitEvent} className="space-y-4">
          {previewUrl && (
            <div>
              <img src={previewUrl} alt="Preview" style={{ height: "200px", width: "auto" }} />
            </div>
          )}
          <div className="flex items-center space-x-2">
            <label htmlFor="image" className="text-gray-700">Obrázek:</label>
            <input
              type="file"
              accept="image/*"
              id="image"
              onChange={handleImageChange}
              className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
              required
            />
          </div>
          <TextField
            label="Název akce"
            value={newEvent.name}
            onChange={(e) => setNewEvent({ ...newEvent, name: e.target.value })}
            fullWidth
            required
          />
          <TextField
            label="Organizátor"
            value={newEvent.organizer}
            onChange={(e) => setNewEvent({ ...newEvent, organizer: e.target.value })}
            fullWidth
            required
          />
          <FormControl fullWidth required>
            <InputLabel>Kraj</InputLabel>
            <Select
              value={newEvent.region}
              onChange={(e) => setNewEvent({ ...newEvent, region: e.target.value })}
            >
              <MenuItem value="praha">Praha</MenuItem>
              <MenuItem value="stredocesky">Středočeský</MenuItem>
              <MenuItem value="jihocesky">Jihočeský</MenuItem>
              <MenuItem value="plzensky">Plzeňský</MenuItem>
              <MenuItem value="karlovarsky">Karlovarský</MenuItem>
              <MenuItem value="ustecky">Ústecký</MenuItem>
              <MenuItem value="liberecky">Liberecký</MenuItem>
              <MenuItem value="kralovehradecky">Královéhradecký</MenuItem>
              <MenuItem value="pardubicky">Pardubický</MenuItem>
              <MenuItem value="vysocina">Vysočina</MenuItem>
              <MenuItem value="jihomoravsky">Jihomoravský</MenuItem>
              <MenuItem value="olomoucky">Olomoucký</MenuItem>
              <MenuItem value="moravskoslezsky">Moravskoslezský</MenuItem>
              <MenuItem value="zlinsky">Zlínský</MenuItem>
            </Select>
          </FormControl>
          <TextField
            label="Město"
            value={newEvent.city}
            onChange={(e) => setNewEvent({ ...newEvent, city: e.target.value })}
            fullWidth
            required
          />
          <TextField
            label="Adresa"
            value={newEvent.address}
            onChange={(e) => setNewEvent({ ...newEvent, address: e.target.value })}
            fullWidth
            required
          />
          <TextField
            label="Datum"
            type="date"
            value={newEvent.date_of_event}
            onChange={(e) => setNewEvent({ ...newEvent, date_of_event: e.target.value })}
            fullWidth
            required
            InputLabelProps={{ shrink: true }}
          />
          <TextField
            label="Popis"
            value={newEvent.description}
            onChange={(e) => setNewEvent({ ...newEvent, description: e.target.value })}
            fullWidth
            required
          />
          <CategoryDropZone onDrop={handleDropCategory} />
          <div className="flex flex-wrap gap-2">
            {EVENT_BUTTONS.map((category) => (
              <DraggableCategory key={category.value} category={category.value} />
            ))}
          </div>
        </form>
      </DialogContent>
      <DialogActions>
        <Button onClick={onClose} color="primary">Zpět</Button>
        <Button onClick={handleSubmitEvent} color="primary" variant="contained">Vytvořit</Button>
      </DialogActions>
    </Dialog>
  );
};

export default NewEventForm;
