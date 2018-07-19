using UnityEngine;
using UnityEngine.SceneManagement;
using System.Collections;

public class PlayerScript : MonoBehaviour
{
    private Rigidbody _rigidbody;
    private GameObject _meshChild;

    public float WalkSpeed = 22.0f;
    public bool CanMove = true;

    public float Gravity = -20.0f;
    private float _upForceY;
    public float DistanceFromMiddleToBot = 0.5f;
    public float BalanceHeight = 1.75f;
    private float _wave = 0.0f;
    public float DampForce = 10.0f;

    public float PlayerWidth = 0.55f;

    //Charged jump
    private float _jumpSpeed = 0.0f;
    private const float _normalJumpSpeed = 12.0f;
    private float _jumpChargeSpeed = 10.0f;
    private float _maxJumpSpeed = 20.0f;
    private const float _smallTimeFrameNoJump = 0.4f;
    private float _timeCheckJump;

    private Vector3 _belowPosPrev = Vector3.zero;
    GameObject belowGameObjectPrev = null;


    // Use this for initialization
    void Start()
    {
        _rigidbody = GetComponent<Rigidbody>();
        _meshChild = transform.FindChild("Mesh").gameObject;
        _upForceY = -Gravity;
        _jumpSpeed = _normalJumpSpeed;
        _timeCheckJump = _smallTimeFrameNoJump;
    }

    // Update is called once per frame
    void Update()
    {

        //MOVEMENT
        Vector3 movement = Vector3.zero;
        movement.x += Input.GetAxis("Horizontal") * WalkSpeed;

        //FLOATFORCE
        //**********

        //necessary rays
        Vector3 raysOffset = new Vector3(PlayerWidth, 0, 0);
        LayerMask layerMask = ~(1 << LayerMask.NameToLayer("Player"));
        Ray rayLeft = new Ray(transform.position - raysOffset, Vector3.down);
        Ray rayRight = new Ray(transform.position + raysOffset, Vector3.down);
        RaycastHit rayHitLeft = new RaycastHit();
        RaycastHit rayHitRight = new RaycastHit();
        Physics.Raycast(rayLeft, out rayHitLeft, Mathf.Infinity, layerMask);
        Physics.Raycast(rayRight, out rayHitRight, Mathf.Infinity, layerMask);
 
        RaycastHit rayHit;
        if (rayHitLeft.distance < rayHitRight.distance)
        {
            rayHit = rayHitLeft;
           
        }
        else
        {
            rayHit = rayHitRight;
        }

       
        //UNABLE TO CLIMB STEEP SURFACES
        if (Vector3.Angle(-rayHit.normal, Vector3.down) > 60.0f)
        {
            if (rayHitLeft.distance < rayHitRight.distance)
            {

                movement.x +=  1.0f;
                if (Input.GetAxis("Horizontal") < 0)
                {
                    movement.x += -Input.GetAxis("Horizontal") * WalkSpeed;
                }
            }
            else
            {
                movement.x -=  WalkSpeed + 1.0f;
                if (Input.GetAxis("Horizontal") > 0)
                {
                    movement.x += -Input.GetAxis("Horizontal") * WalkSpeed;
                }
            }
        }

        //moves along with object below ONLY when distance is smaller than 3
        GameObject belowGameObject = rayHit.collider.gameObject;
        Vector3 belowPosNow = belowGameObject.transform.position;
        if ((_belowPosPrev != Vector3.zero) && (belowGameObjectPrev != null))
        {
            if (belowGameObjectPrev == belowGameObject)
            {
                Vector3 belowMovement = belowPosNow - _belowPosPrev;

                if (rayHit.distance < 3.0f)
                {
                    transform.Translate(belowMovement);
                }
                
                //Debug.DrawLine(transform.position, transform.position + belowMovement);
                //Debug.Log(belowMovement);
            }
        
        }

        
        _belowPosPrev = belowPosNow;
        belowGameObjectPrev = belowGameObject;
       

        Vector3 balancePoint = new Vector3(transform.position.x,rayHit.point.y, transform.position.z);
        balancePoint.y += BalanceHeight;
        

        //floatForce
        if (transform.position.y < balancePoint.y)
        {
            
            movement.y += _upForceY + ((balancePoint.y - transform.position.y)*2.0f );
        }
        else
        {
            //Debug.Log("pos" + transform.position.y + "balancepos:" + balancePoint);
        }


        if (rayHit.distance < (DistanceFromMiddleToBot + 0.3f)
        && _rigidbody.velocity.y < 0)
        {
            movement.y += -_rigidbody.velocity.y * DampForce;
        }

        //CHARGED JUMPING
        if (Input.GetButton("Jump") && _timeCheckJump > _smallTimeFrameNoJump && transform.position.y < balancePoint.y + 0.5f)
        {
            movement.y -= 1.5f;

            if (_jumpSpeed < _maxJumpSpeed)
            {
                _jumpSpeed += Time.deltaTime * _jumpChargeSpeed;

                this.transform.localScale = new Vector3(this.transform.localScale.x + Time.deltaTime * 0.5f, this.transform.localScale.y - Time.deltaTime * 0.5f, this.transform.localScale.z);
            }
            
        }
        else if (Input.GetButtonUp("Jump") && _timeCheckJump > _smallTimeFrameNoJump  && transform.position.y < balancePoint.y + 0.5f) 
        {
            GetComponent<Rigidbody>().AddForce(Vector3.up*_jumpSpeed, ForceMode.Impulse);
            _jumpSpeed = _normalJumpSpeed;

            _timeCheckJump = 0.0f;

            GetComponent<SoundPlayer>().PlayOneShot(SoundPlayer.SoundToPlay.Jump);
            //Debug.Log("jump");

            this.transform.localScale = new Vector3(1, 1, 1);
        }

        if (_timeCheckJump <= _smallTimeFrameNoJump)
        {
            _timeCheckJump += Time.deltaTime;
        }

        //GRAVITY
        movement.y += Gravity;

        if (CanMove)
        {
            _rigidbody.AddForce(movement);
        }
        

        //MESH MOVEMENT (Smooth flowing movement even when standing still)       
        Vector3 vec = _meshChild.transform.position;
        //Debug.DrawLine(transform.position, balancePoint, Color.red);

        if (Mathf.Abs(transform.position.y - balancePoint.y) < 0.3 || _meshChild.transform.localPosition.y < 0.3f)
        {
            _wave += Time.deltaTime * 2;
            vec.y += Mathf.Sin(_wave) / 100.0f;

            _meshChild.transform.position = vec;
        }


        if (Input.GetKeyDown(KeyCode.R))
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().name);
        }

        if (Input.GetKeyUp(KeyCode.E))
        {
            GetComponent<SoundPlayer>().PlayOneShot(SoundPlayer.SoundToPlay.Interact);
            
        }

    }
}


