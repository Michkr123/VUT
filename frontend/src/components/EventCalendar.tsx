import { useState, useEffect } from 'react';
import { Add, Person } from '@mui/icons-material';
import {
  Avatar,
  Button,
  DialogActions,
  DialogContent,
  DialogContentText,
  List,
  ListItem,
  ListItemAvatar,
  ListItemText,
  TextField,
} from '@mui/material';
import moment from 'moment';
import { EventCalendar } from 'react-mui-event-calendar';


const emails = ['username@gmail.com', 'user02@gmail.com']

function App() {
  const data = [
    {
      date: new Date(),
      title: 'First',
      popupContent: (
        <>
          <List sx={{ pt: 0 }}>
            {emails.map((email) => (
              <ListItem button key={email}>
                <ListItemAvatar>
                  <Avatar>
                    <Person />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary={email} />
              </ListItem>
            ))}

            <ListItem autoFocus button>
              <ListItemAvatar>
                <Avatar>
                  <Add />
                </Avatar>
              </ListItemAvatar>
              <ListItemText primary='Add account' />
            </ListItem>
          </List>
        </>
      ),
      id: '1',
    },
    {
      date: moment().subtract(2, 'days'),
      title: "Use Google's location service?",
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText id='alert-dialog-description'>
              Let Google help apps determine location. This means sending
              anonymous location data to Google, even when no apps are running.
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Disagree</Button>
            <Button>Agree</Button>
          </DialogActions>
        </>
      ),
      id: '2',
    },
    {
      date: moment().subtract(3, 'days'),
      title: 'Third',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              To subscribe to this website, please enter your email address
              here. We will send updates occasionally.
            </DialogContentText>
            <TextField
              autoFocus
              margin='dense'
              id='name'
              label='Email Address'
              type='email'
              fullWidth
              variant='standard'
            />
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      color: '#000',
      id: '3',
    },
    {
      date: new Date(),
      title: 'Fourth',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              {[...new Array(50)]
                .map(
                  () => `Cras mattis consectetur purus sit amet fermentum.
Cras justo odio, dapibus ac facilisis in, egestas eget quam.
Morbi leo risus, porta ac consectetur ac, vestibulum at eros.
Praesent commodo cursus magna, vel scelerisque nisl consectetur et.`
                )
                .join('\n')}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      color: '#ffe100',
      id: '4',
    },
    {
      date: moment().subtract(10, 'days'),
      title: 'Fourth',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              {[...new Array(50)]
                .map(
                  () => `Cras mattis consectetur purus sit amet fermentum.
Cras justo odio, dapibus ac facilisis in, egestas eget quam.
Morbi leo risus, porta ac consectetur ac, vestibulum at eros.
Praesent commodo cursus magna, vel scelerisque nisl consectetur et.`
                )
                .join('\n')}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      color: '#ffe100',
      id: '4',
    },
    {
      date: moment().subtract(30, 'days'),
      title: 'Fourth',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              {[...new Array(50)]
                .map(
                  () => `Cras mattis consectetur purus sit amet fermentum.
Cras justo odio, dapibus ac facilisis in, egestas eget quam.
Morbi leo risus, porta ac consectetur ac, vestibulum at eros.
Praesent commodo cursus magna, vel scelerisque nisl consectetur et.`
                )
                .join('\n')}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      id: '4',
    },
    {
      date: new Date(),
      title: 'Fourth',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              {[...new Array(50)]
                .map(
                  () => `Cras mattis consectetur purus sit amet fermentum.
Cras justo odio, dapibus ac facilisis in, egestas eget quam.
Morbi leo risus, porta ac consectetur ac, vestibulum at eros.
Praesent commodo cursus magna, vel scelerisque nisl consectetur et.`
                )
                .join('\n')}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      color: '#ffe100',
      id: '4',
    },
    {
      date: moment().add(5, 'days'),
      title: 'Fourth',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              {[...new Array(50)]
                .map(
                  () => `Cras mattis consectetur purus sit amet fermentum.
Cras justo odio, dapibus ac facilisis in, egestas eget quam.
Morbi leo risus, porta ac consectetur ac, vestibulum at eros.
Praesent commodo cursus magna, vel scelerisque nisl consectetur et.`
                )
                .join('\n')}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      color: '#ffe100',
      id: '4',
    },
    {
      date: new Date(),
      title: 'Fourth',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              {[...new Array(50)]
                .map(
                  () => `Cras mattis consectetur purus sit amet fermentum.
Cras justo odio, dapibus ac facilisis in, egestas eget quam.
Morbi leo risus, porta ac consectetur ac, vestibulum at eros.
Praesent commodo cursus magna, vel scelerisque nisl consectetur et.`
                )
                .join('\n')}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      color: '#ffe100',
      id: '4',
    },
    {
      date: moment().subtract(2, 'days'),
      title: 'Fourth',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              {[...new Array(50)]
                .map(
                  () => `Cras mattis consectetur purus sit amet fermentum.
Cras justo odio, dapibus ac facilisis in, egestas eget quam.
Morbi leo risus, porta ac consectetur ac, vestibulum at eros.
Praesent commodo cursus magna, vel scelerisque nisl consectetur et.`
                )
                .join('\n')}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      color: '#ffe100',
      id: '4',
    },
    {
      date: moment().add(30, 'days'),
      title: 'Fourth',
      popupContent: (
        <>
          <DialogContent>
            <DialogContentText>
              {[...new Array(50)]
                .map(
                  () => `Cras mattis consectetur purus sit amet fermentum.
Cras justo odio, dapibus ac facilisis in, egestas eget quam.
Morbi leo risus, porta ac consectetur ac, vestibulum at eros.
Praesent commodo cursus magna, vel scelerisque nisl consectetur et.`
                )
                .join('\n')}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button>Cancel</Button>
            <Button>Subscribe</Button>
          </DialogActions>
        </>
      ),
      color: '#ffe100',
      id: '4',
    },
  ]

  const [dataSource, setDataSource] = useState(data)

  return (
    <div
      style={{
        width: '100%',
        minHeight: '100vh',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        margin: '3em 0',
      }}
    >
      <EventCalendar
        dataSource={dataSource}
        pallet={{ primary: '#32d3a2', secondary: '#2343d3' }}
        onDataChange={(newEvents) => setDataSource(newEvents)}
      />
    </div>
  )
}

export default App